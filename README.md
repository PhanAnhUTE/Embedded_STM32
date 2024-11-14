###### EMBEDDED 


<details>
    <summary>LESSION 1: GPIO </summary>
  
    GPIO trên bộ thư viện SPL (Standard Peripherals Firmware Library ). Các hàm phục vụ cho việc cấu hình GPIO, cấp xung ngoại vi được định nghĩa trong file "stm32f10x_rcc.h", và "stm32f10x_gpio.h"
  trong thư viện này, các cấu hình được chia thành các trường và định nghĩa bằng các cấu trúc như struct và enum.

    Một số định nghĩa đã được cung cấp trong thư viện có thể kể ra như:
    
    Các GPIO: các thanh ghi phục vụ chức năng GPIO đã được tổ chức dưới dạng 1 struct. Các thanh ghi cấu hình cho các GPIO được tổ chức trong struct GPIO_Typedef.
    typedef struct
    {
      __IO uint32_t CRL;
      __IO uint32_t CRH;
      __IO uint32_t IDR;
      __IO uint32_t ODR;
      __IO uint32_t BSRR;
      __IO uint32_t BRR;
      __IO uint32_t LCKR;
    } GPIO_TypeDef;
    Trong thư viện SPL, các thuộc tính của GPIO được tổ chức thành 1 struct GPIO_InitTypeDef chứa các trường GPIO_Mode, GPIO_Pin và GPIO_Speed.
    typedef struct
    {
      uint16_t GPIO_Pin;             /*!< Specifies the GPIO pins to be configured.
                                          This parameter can be any value of @ref GPIO_pins_define */
    
      GPIOSpeed_TypeDef GPIO_Speed;  /*!< Specifies the speed for the selected pins.
                                          This parameter can be a value of @ref GPIOSpeed_TypeDef */
    
      GPIOMode_TypeDef GPIO_Mode;    /*!< Specifies the operating mode for the selected pins.
                                          This parameter can be a value of @ref GPIOMode_TypeDef */
    }GPIO_InitTypeDef;
    GPIO_InitStructure.GPIO_Mode là một trường dùng để xác định chế độ hoạt động của chân GPIO trong thư viện của STM32.
    typedef enum
    { GPIO_Mode_AIN = 0x0,
      GPIO_Mode_IN_FLOATING = 0x04,
      GPIO_Mode_IPD = 0x28,
      GPIO_Mode_IPU = 0x48,
      GPIO_Mode_Out_OD = 0x14,
      GPIO_Mode_Out_PP = 0x10,
      GPIO_Mode_AF_OD = 0x1C,
      GPIO_Mode_AF_PP = 0x18
    }GPIOMode_TypeDef;
    *Dưới đây là các giá trị mà trường GPIO_Mode có thể nhận và giải thích chi tiết về từng chế độ:
    
    GPIO_Mode_AIN:
    
    ● Mô tả: Analog Input.
    
    ● Giải thích: Chân GPIO được cấu hình làm đầu vào analog. Thường được sử dụng cho các chức năng như ADC (Analog to Digital Converter).
    
    GPIO_Mode_IN_FLOATING:
    
    ● Mô tả: Floating Input.
    
    ● Giải thích: Chân GPIO được cấu hình làm đầu vào và ở trạng thái nổi (không pull-up hay pull-down). Điều này có nghĩa là chân không được kết nối cố định với mức cao (VDD) hoặc mức thấp (GND) thông qua điện trở.
    
    GPIO_Mode_IPD:
    
    ● Mô tả: Input with Pull-down.
    
    ● Giải thích: Chân GPIO được cấu hình làm đầu vào với một điện trở pull-down nội bộ kích hoạt. Khi không có tín hiệu nào được áp dụng lên chân này, nó sẽ được kéo về mức thấp (GND).
    
    GPIO_Mode_IPU:
    
    ● Mô tả: Input with Pull-up.
    
    ● Giải thích: Chân GPIO được cấu hình làm đầu vào với một điện trở pull-up nội bộ kích hoạt. Khi không có tín hiệu nào được áp dụng lên chân này, nó sẽ được kéo về mức cao (VDD).
    
    GPIO_Mode_Out_OD:
    
    ● Mô tả: Open-drain Output.
    
    ● Giải thích: Chân GPIO được cấu hình làm đầu ra với chế độ open-drain. Trong chế độ này, chân có thể được kéo xuống mức thấp, nhưng để đạt được mức cao, cần một điện trở pull-up ngoài hoặc từ một nguồn khác.
    
    GPIO_Mode_Out_PP:
    
    ● Mô tả: Push-pull Output.
    
    ● Giải thích: Chân GPIO được cấu hình làm đầu ra với chế độ push-pull. Trong chế độ này, chân có thể đạt được cả mức cao và mức thấp mà không cần bất kỳ phần cứng bổ sung nào.
    
    GPIO_Mode_AF_OD:
    
    ● Mô tả: Alternate Function Open-drain.
    
    ● Giải thích: Chân GPIO được cấu hình để hoạt động trong một chức năng thay thế (như USART, I2C, etc.) và sử dụng chế độ open-drain.
    
    GPIO_Mode_AF_PP:
    
    ● Mô tả: Alternate Function Push-pull.
    
    ● Giải thích: Chân GPIO được cấu hình để hoạt động trong một chức năng thay thế và sử dụng chế độ push-pull.
    
    GPIO_Pin là trường xác định chân trong GPIOx tương ứng. các giá trị được khai báo trong file header, có dạng GPIO_Pin_x với x là chân từ 0-15.
    
    #define GPIO_Pin_0                                        ((unint16_t)0x0001)  /* < Pin 0 selected */
    #define GPIO_Pin_1                                        ((unint16_t)0x0002)  /* < Pin 1 selected */
    ......
    #define GPIO_Pin_All                                      ((unint16_t)0xFFFF)  /* < All Pin selected */


    GPIO_Speed là trường xác định tốc độ đáp ứng của chân. Thường được cấu hình đi kèm với chế độ Output, các giá trị cũng được khai báo trong file header trong GPIO_SpeedTypeDef:

    typedef enum
    { 
      GPIO_Speed_10MHz = 1,
      GPIO_Speed_2MHz, 
      GPIO_Speed_50MHz
    }GPIOSpeed_TypeDef;
    Cấu hình RCC cấp clock cho ngoại vi
    
    Trong tài liệu của bộ thư viện : “STM32F10x Standard Peripherals Firmware Library”, xung clock cho ngoại vi được cấu hình bởi các hàm trong modules RCC.
    
    Các hàm :có chức năng cấp xung hoặc ngưng cấp xung cho ngoại vi tương ứng. Các hàm này được định nghĩa trong file "stm32f10x_rcc.h". Các hàm này nhận tham số vào là Macro của các ngoại vi được định nghĩa sẵn trong file header, tham số thứ 2 quy định việc cấp hay ngưng xung clock cho ngoại vi tương ứng.
    
    RCC_APB1PeriphClockCmd
    Enables or disables the Low Speed APB (APB1) peripheral clock.
    
    Parameters:
    
    RCC_APB1Periph,:specifies the APB1 peripheral to gates its clock. This parameter can be any combination of the following values:
    
    RCC_APB1Periph_TIM2, 
    RCC_APB1Periph_TIM3, 
    RCC_APB1Periph_TIM4, 
    RCC_APB1Periph_TIM5, 
    RCC_APB1Periph_TIM6, 
    RCC_APB1Periph_TIM7, 
    RCC_APB1Periph_WWDG, 
    RCC_APB1Periph_SPI2, 
    RCC_APB1Periph_SPI3, 
    RCC_APB1Periph_USART2, 
    RCC_APB1Periph_USART3, 
    RCC_APB1Periph_USART4, 
    RCC_APB1Periph_USART5, 
    RCC_APB1Periph_I2C1, 
    RCC_APB1Periph_I2C2, 
    RCC_APB1Periph_USB, 
    RCC_APB1Periph_CAN1, 
    RCC_APB1Periph_BKP, 
    RCC_APB1Periph_PWR, 
    RCC_APB1Periph_DAC, 
    RCC_APB1Periph_CEC, 
    RCC_APB1Periph_TIM12, 
    RCC_APB1Periph_TIM13, 
    RCC_APB1Periph_TIM14
    
    NewState,:
    new state of the specified peripheral clock. This parameter can be: ENABLE or DISABLE.
    
    Return values:
    None
    
    RCC_APB2PeriphClockCmd
    Enables or disables the High Speed APB (APB2) peripheral clock. Parameters:
    
    RCC_APB2Periph,:specifies the APB2 peripheral to gates its clock. This parameter can be any combination of the following values:
    
    RCC_APB2Periph_AFIO, 
    RCC_APB2Periph_GPIOA, 
    RCC_APB2Periph_GPIOB, 
    RCC_APB2Periph_GPIOC, 
    RCC_APB2Periph_GPIOD, 
    RCC_APB2Periph_GPIOE, 
    RCC_APB2Periph_GPIOF, 
    RCC_APB2Periph_GPIOG, 
    RCC_APB2Periph_ADC1, 
    RCC_APB2Periph_ADC2, 
    RCC_APB2Periph_TIM1, 
    RCC_APB2Periph_SPI1, 
    RCC_APB2Periph_TIM8, 
    RCC_APB2Periph_USART1, 
    RCC_APB2Periph_ADC3, 
    RCC_APB2Periph_TIM15, 
    RCC_APB2Periph_TIM16, 
    RCC_APB2Periph_TIM17, 
    RCC_APB2Periph_TIM9, 
    RCC_APB2Periph_TIM10, 
    RCC_APB2Periph_TIM11
    NewState,:
    new state of the specified peripheral clock. This parameter can be: ENABLE or DISABLE.
    
    Return values:
    None
    GPIOC được cấp xung bởi APB2 nên sử dụng hàm :
    
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE); để cấu hình clock.
    
    Cấu hình Pin GPIO
    
    Như đã đề cập ở trên, các thuộc tính của 1 chân trong GPIO có thể được cấu hình thông qua struct GPIO_InitTypeDef, chúng ta sẽ tạo 1 biến struct kiểu này, sau đó gán các giá trị cần cấu hình thông qua biến đó.
    
    Khởi tạo GPIO Hàm GPIO_Init(); khởi tạo GPIOx với các tham số đã được thiết lập trong GPIO_InitStruct. Hàm nhận 2 tham số là 1 GPIOx cần khởi tạo và 1 con trỏ trỏ tới struct GPIO_InitTypedDef chứa các thông tin đã thiết lập cho GPIO.
    
    Vì vậy, để khởi tạo 1 GPIO để sử dụng, trước tiên cần cấu hình clock, sau đó tạo 1 struct GPIO_InitTypedDef cấu hình tham số cho GPIO, sau đó gọi hàm GPIO_Init() với GPIOx cần cấu hình và struct vừa tạo.
    
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APBxPeriph_GPIOx, ENABLE);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_x;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_xx;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_xx;
    GPIO_Init(GPIOx, &GPIO_InitStruct);
    2 Các hàm cơ bản trên GPIO.
    
    Thư viện SPL hỗ trợ sẵn các hàm để thực thi trên các GPIO.
    
    GPIO_SetBits(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin)*
    ● Mô tả: Đặt một hoặc nhiều chân GPIO ở mức cao (logic 1).
    
    ● Tham số:
    
    ● GPIOx: là cổng GPIO muốn điều khiển (ví dụ: GPIOA, GPIOB,...).
    
    ● GPIO_Pin: chọn chân hoặc chân cần đặt ở mức cao (ví dụ: GPIO_Pin_0, GPIO_Pin_1 hoặc kết hợp như GPIO_Pin_0 | GPIO_Pin_1).
    
     GPIO_ResetBits(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin)*
    ● Mô tả: Đặt một hoặc nhiều chân GPIO ở mức thấp (logic 0).
    
    ● Tham số: Tương tự như hàm GPIO_SetBits.
    
    GPIO_ReadInputDataBit(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin)*
    ● Mô tả: Đọc trạng thái của một chân GPIO đã được cấu hình là input.
    
    ● Tham số: Tương tự như hàm GPIO_SetBits.
    
    ● Giá trị trả về: Trả về Bit_SET nếu chân đang ở mức cao hoặc Bit_RESET nếu chân đang ở mức thấp.
    
    GPIO_ReadOutputDataBit(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin)*
    ● Mô tả: Đọc trạng thái của một chân GPIO đã được cấu hình là output.
    
    ● Tham số: Tương tự như hàm GPIO_SetBits.
    
    ● Giá trị trả về: Trả về Bit_SET nếu chân đang ở mức cao hoặc Bit_RESET nếu chân đang ở mức thấp.
    
    GPIO_WriteBit(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin, BitAction BitVal)*
    ● Mô tả: Đặt trạng thái của một chân GPIO dựa trên giá trị của BitVal.
    
    ● Tham số:
    
    ● GPIOx và GPIO_Pin tương tự như hàm GPIO_SetBits.
    
    ● BitVal: là giá trị mà bạn muốn đặt cho chân GPIO, có thể là Bit_SET hoặc Bit_RESET.
    
     GPIO_ReadInputData(GPIO_TypeDef GPIOx)*
    ● Mô tả: Đọc giá trị của tất cả các chân GPIO đã được cấu hình là đầu vào trên cổng GPIO chỉ định.
    
    ● Tham số:
    
    ● GPIOx: cổng GPIO mà bạn muốn đọc (ví dụ: GPIOA, GPIOB,...).
    
    ● Giá trị trả về: Một giá trị 16-bit biểu diễn trạng thái của tất cả các chân trên cổng GPIO.
    
     GPIO_ReadOutputData(GPIO_TypeDef GPIOx)*
    ● Mô tả: Đọc giá trị của tất cả các chân GPIO đã được cấu hình là đầu ra trên cổng GPIO chỉ định.
    
    ● Tham số:
    
    ● GPIOx: cổng GPIO mà bạn muốn đọc.
    
    ● Giá trị trả về: Một giá trị 16-bit biểu diễn trạng thái của tất cả các chân trên cổng GPIO.
    
     GPIO_Write(GPIO_TypeDef GPIOx, uint16_t PortVal)*
    ● Mô tả: Ghi giá trị cho toàn bộ cổng GPIO.
    
    ● Tham số:
    
    ● GPIOx: cổng GPIO bạn muốn ghi.
    
    ● PortVal: giá trị 16-bit mà bạn muốn đặt cho cổng GPIO.
    
     GPIO_PinLockConfig(GPIO_TypeDef GPIOx, uint16_t GPIO_Pin)*
    ● Mô tả: Khóa cấu hình của chân GPIO. Sau khi chân đã bị khóa, bạn sẽ không thể thay đổi cấu hình của nó cho đến khi hệ thống được reset.
    
    ● Tham số:
    
    ● GPIOx: cổng GPIO mà bạn muốn khóa chân.
    
    ● GPIO_Pin: chọn chân cần khóa (ví dụ: GPIO_Pin_0, GPIO_Pin_1 hoặc kết hợp như GPIO_Pin_0 | GPIO_Pin_1).
    
    GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
    ● Mô tả: Cấu hình chân sự kiện đầu ra.
    
    ● Tham số:
    
    ● GPIO_PortSource: xác định cổng GPIO.
    
    ● GPIO_PinSource: xác định chân GPIO.
    
     GPIO_EventOutputCmd(FunctionalState NewState)
    ● Mô tả: Cho phép hoặc vô hiệu hóa chân sự kiện đầu ra.
    
    ● Tham số:
    
    ● NewState: trạng thái mới của chân. Có thể là ENABLE hoặc DISABLE.
    
     GPIO_AFIODeInit()
    ● Mô tả: Đặt lại tất cả các thanh ghi của AFIO (Alternate Function IO) về giá trị mặc định.

    



</details>

<details>
    <summary>LESSION 2: Ngắt(Interrupt) &Timer </summary>

## Interupt(Ngắt)
    ● Ngắt là 1 sự kiện khẩn cấp xảy ra trong hay ngoài vi điều khiển. Nó yêu MCU phải dừng chương trình chính và thực thi chương trình ngắt.


    ● Mỗi ngắt có 1 trình phục vụ ngắt, sẽ yêu cầu MCU thực thi lệnh tại trình phục vụ ngắt khi có ngắt xảy ra
    
    ● Các ngắt có các địa chỉ cố định trong bộ nhớ để giữ các trình phục vụ. Các địa chỉ này gọi là vector ngắt

## Ngắt                    Cờ ngắt            Địa chỉ trình phục vụ ngắt            Độ ưu tiên ngắt
  Reset                       -                        0000h                                  
  Ngắt ngoài                 IE0                       0003h                        Lập trình được
  Timer1                     TF1                       001Bh                        Lập trình được
  Ngắt truyền thông                                                                 Lập trình được

Quá trình thực thi ngắt:
                        //main.c
    0xC1                  main(){
    0xC2                  while(1){
    ...                 		//do something
    0xC9                   }
                        }
                        
    0xB1-0xB5           ISR(); // ngat ngoai
    0xC3                ISR1();

 ● B1: Xử lý xong câu lệnh đang chạy. (Quá trình thực thi câu lệnh mã máy qua các bước: lấy lệnh từ bộ nhớ FLASH, giải mã lệnh, thực thi lệnh)
 
 ● B2: Lưu địa chỉ câu lệnh tiếp theo, lưu trạng thái hoạt động của VDK (các cờ, trạng thái năng lượng) vào vùng nhớ STACK
 
 ● B3: VDK tắt bit ngắt toàn cục để ngăn chặn các ngắt khác can thiệp trong khi xử lý ngắt hiện tại. Nếu vi điều khiển đang ở chế độ tiết kiệm năng lượng, nó sẽ chuyển sang chế độ hoạt động bình thường

 ● B4: Thực thi ngắt bằng cách Vi điều khiển nạp địa chỉ của chương trình phục vụ ngắt (Interrupt Service Routine - ISR) từ bảng vectơ ngắt vào thanh ghi PC

 ● B5: Thực thi xong sẽ thực hiện quá trình phục hồi ngữ cảnh (unstacking)

 Lưu ý: Thời gian xử lý ngắt rất nhỏ (0.005 us) nên khoảng thời gian xử lý ngắt đến câu lệnh tiếp theo là rất nhanh

 Nếu có nhiều ngắt xảy ra thì VDK sẽ dựa vào mức độ ưu tiên ngắt (số càng nhỏ mức ưu tiên càng cao) để thực thi theo thứ tự.

 Nếu ngắt có cùng độ ưu tiên thì VDK sẽ xét theo sub priotiy (độ ưu tiên phụ).

## Các loại ngắt thông dụng
       ● Ngắt ngoài
            Xảy ra khi có thay đổi điện áp trên các chân GPIO được cấu hình làm ngõ vào ngắt.

            LOW: kích hoạt ngắt liên tục khi chân ở mức thấp.
            
            HIGH: Kích hoạt liên tục khi chân ở mức cao.
            
            Rising: Kích hoạt khi trạng thái trên chân chuyển từ thấp lên cao.
            
            Falling: Kích hoạt khi trạng thái trên chân chuyển từ cao xuống thấp

        ● Ngắt timer
            Ngắt Timer xảy ra khi giá trị trong thanh ghi đếm của timer tràn. Giá trị tràn được xác định bởi giá trị cụ thể trong thanh ghi đếm của timer.
            Vì đây là ngắt nội trong MCU, nên phải reset giá trị thanh ghi timer để có thể tạo được ngắt tiếp theo

        ● Ngắt truyền nhận

            Ngắt truyền nhận xảy ra khi có sự kiện truyền/nhận dữ liệu giữ MCU với các thiết bị bên ngoài hay với MCU. Ngắt này sử dụng cho nhiều phương thức như Uart, SPI, I2C…v.v nhằm đảm bảo việc truyền nhận chính xác

        ● Độ ưu tiên ngắt

            Độ ưu tiên ngắt là khác nhau ở các ngắt. Nó xác định ngắt nào được quyền thực thi khi nhiều ngắt xảy ra đồng thời
            
            STM32 quy định ngắt nào có số thứ tự ưu tiên càng thấp thì có quyền càng cao. Các ưu tiên ngắt có thể lập trình được
            

</details>




























