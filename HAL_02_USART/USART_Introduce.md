一.串口调试助手发送消息：
串口助手(PC)  →  USB转TTL芯片  →  一根线(TX)  →  MCU 的 PA10(RX) 引脚
  ① 硬件自主收数（CPU 完全不参与）

  USART 外设靠波特率发生器按 115200 的节奏去采样 PA10 引脚，把电平变化一个 bit 一个 bit 地「移位」进移位寄存器。起始位、8
  个数据位、停止位，全程是硬件完成的，你的程序此刻在干嘛都无所谓。

  ② 收满一个字节 → 硬件置标志
  当一个完整字节（一帧）收完后，硬件把数据从移位寄存器搬到数据寄存器 DR，并自动把状态寄存器里的 SR.RXNE 置 1（RXNE = Receive data
  register Not Empty，接收寄存器非空）。

  ③ 中断触发有条件
  RXNE=1 之后，会不会跳进中断，取决于 CR1.RXNEIE 这个中断使能位：
  ┌───────────────┬────────────────────────────────────────────────────────┐
  │    RXNEIE     │                          结果                          │
  ├───────────────┼────────────────────────────────────────────────────────┤
  │ = 1（已使能） │ 触发中断 → NVIC → 跳进 USART1_IRQHandler               │
  ├───────────────┼────────────────────────────────────────────────────────┤
  │ = 0（未使能） │ 不触发中断，只默默置 RXNE 标志，你得自己去轮询 SR.RXNE │
  └───────────────┴────────────────────────────────────────────────────────┘

  所以之前我一直强调的那个 HAL_UART_Receive_IT(...) 到底干了什么，现在能对应上了：

  HAL_UART_Receive_IT(&huart1, g_rx_buffer, RXBUFFERSIZE);
     └─ 内部调 UART_Start_Receive_IT()
          └─ __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);   // 就是把 CR1.RXNEIE 置 1

  这一句就是打开「收满字节就触发中断」的开关。 没有它，RXNE 会置位，但永远不产生中断——这正是你之前「没反应」的原因之一。

  还有一道总开关

  光有 RXNEIE 还不够，中断还得能到 CPU。这靠 NVIC（嵌套向量中断控制器）放行，在 HAL_UART_MspInit 里已经做好了：

  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);      // 让 USART1 的中断能上报给 CPU

  一句话总结

  串口助手发数据 → 硬件按 bit 收 → 每收满 1 字节硬件置 RXNE → RXNEIE 使能时 → NVIC → USART1_IRQHandler → HAL_UART_IRQHandler →
  UART_Receive_IT → 你的 HAL_UART_RxCpltCallback。

  注意中断是按字节触发的（一个字节一次），不是按「发送动作」或「一行字符串」触发。你发 5 个字符，就会走 5 次这个流程。
