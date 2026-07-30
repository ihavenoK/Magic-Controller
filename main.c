#include "CyberryPotter.h"
#include "weights.h"
#include "nnom.h"
#include "IR.h"

extern Cyberry_Potter_Status_Typedef Cyberry_Potter_Status;
//extern uint16_t IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
//extern float IMU_Data_ACC[3*IMU_SEQUENCE_LENGTH_MAX];
extern float IMU_Data_mGyro[IMU_SEQUENCE_LENGTH_MAX][3];
extern float IMU_Data_mAcc[IMU_SEQUENCE_LENGTH_MAX][3];
int8_t model_output = -1;
#define SCALE (pow(2,INPUT_1_OUTPUT_DEC))

typedef enum eModel_Output{
	Unrecognized = -1,
	RightAngle = 0,
	SharpAngle = 1,
	Lightning = 2,
	Triangle = 3,
	Letter_h = 4,
	letter_R = 5,
	letter_W = 6,
	letter_phi = 7,
	Circle = 8,
	UpAndDown = 9,
	Horn = 10,
	Wave = 11,
	NoMotion = 12
}eModel_Output;

#ifdef NNOM_USING_STATIC_MEMORY
	uint8_t static_buf[1024 * 8];
#endif //NNOM_USING_STATIC_MEMORY

void model_feed_data(void)
{
	const double scale = SCALE;
	uint16_t i = 0;
	for(i = 0; i < IMU_SEQUENCE_LENGTH_MAX;i++){
		nnom_input_data[i*3] = (int8_t)round(IMU_Data_mAcc[i][0] * scale);     // 使用加速度数据 (CSV列0,1,2) 作为CNN输入，与训练时 DEF_USE_COLS=(0,1,2) 保持一致
		nnom_input_data[i*3+1] = (int8_t)round(IMU_Data_mAcc[i][1] * scale);
		nnom_input_data[i*3+2] = (int8_t)round(IMU_Data_mAcc[i][2] * scale);
	}
}

int8_t model_get_output(void)
{
	uint8_t i = 0;
	int8_t max_output = -128;
	int8_t ret = 0;
	for(i = 0; i < 13;i++){
		printf("Output[%d] = %.2f %%\n",i,(nnom_output_data[i] / 127.0)*100);
		if(nnom_output_data[i] >= max_output){
			max_output = nnom_output_data[i] ;
			ret = i;
		}
	}
	if(max_output >= OUPUT_THRESHOLD){
		ret = ret;
	}
	else{
		ret = -1;
	}
	
	switch(ret){
		case Unrecognized:
			printf("Unrecognized");
			break;
		case RightAngle:
			printf("RightAngle");
			break;
		case SharpAngle:
			printf("SharpAngle");
			break;
		case Lightning:
			printf("Lightning");
			break;
		case Triangle:
			printf("Triangle");
			break;
		case Letter_h:
			printf("Letter_h");
			break;
		case letter_R:
			printf("Letter_R");
			break;
		case letter_W:
			printf("Letter_W");
			break;
		case letter_phi:
			printf("Letter_phi");
			break;
		case Circle:
			printf("Circle");
			break;
		case UpAndDown:
			printf("UpAndDown");
			break;
		case Horn:
			printf("Horn");
			break;
		case Wave:
			printf("Wave");
			break;
		case NoMotion:
			printf("Unrecognized");
			break;
	}
	printf("\n");
	return ret;
}

void Model_LED(int8_t model_output)
{
	switch(model_output){
		case RightAngle:
			printf("RightAngle");
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);
			break;
		case SharpAngle:
			printf("SharpAngle");
			GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
			break;
		case Lightning:
			printf("Lightning");
			GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
			break;
		case Triangle:
			printf("Triangle");
			GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
			break;
		case Letter_h:
			printf("Letter_h");
			GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
			break;
		case letter_R:
			printf("letter_R");
			GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
			break;
		case letter_W:
			printf("letter_W");
			GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_SET);
			break;
		case letter_phi:
			printf("letter_phi");
			GPIO_WriteBit(GPIOA,GPIO_Pin_3,Bit_RESET);
			break;
	}
}


int  main(void)
{       
        System_Init();
        printf("Hello!\n");
        GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
	#ifdef NNOM_USING_STATIC_MEMORY
		nnom_set_static_buf(static_buf, sizeof(static_buf)); 
	#endif //NNOM_USING_STATIC_MEMORY
	nnom_model_t* model;
	model = nnom_model_create();
	
	while(1){
		if(Cyberry_Potter_Status.System_Mode == SYSTEM_IR_LEARN){
			// 平时等待监听：保持 LED 熄灭，避免误导
			LED_OFF;

			// RAW 学习：录制原始脉冲
			IR_Raw_Signal_t raw_sig = {0};
			if(IR_Receive_Raw(&raw_sig)){
				// 只有比已存的多才覆盖，避免差录制冲掉好的
				IR_Raw_Signal_t old_sig;
				uint8_t overwrite = 1;
				if(IR_Flash_Load(Cyberry_Potter_Status.Ir_Learn_Slot, &old_sig)){
					if(old_sig.edge_count >= raw_sig.edge_count){
						overwrite = 0;
						printf("IR: kept old edges=%d > new=%d\n",
							old_sig.edge_count, raw_sig.edge_count);
					}
				}
				if(overwrite){
					IR_Flash_Save(Cyberry_Potter_Status.Ir_Learn_Slot, &raw_sig);
				}
				printf("Learned slot %d: edges=%d [",
					Cyberry_Potter_Status.Ir_Learn_Slot, raw_sig.edge_count);
				// 打印前 8 个边沿用于对比
				uint8_t n = raw_sig.edge_count;
				if(n > 8) n = 8;
				for(uint8_t k = 0; k < n; k++){
					printf("%d%s", raw_sig.edges[k], (k+1 < n) ? "," : "");
				}
				printf("%s]\n", raw_sig.edge_count > 8 ? ",..." : "");
				
				// 明确的【成功反馈】：快闪 4 次 + 常亮 1 秒
				for(int b = 0; b < 4; b++){
					LED_ON;  Delay_ms(80);
					LED_OFF; Delay_ms(80);
				}
				LED_ON;
				Delay_ms(1000);
				LED_OFF;
			}


			static uint8_t listen_count = 0;
			listen_count++;
			if(listen_count >= 25){
				listen_count = 0;
				printf("[IR] listening... slot=%d\n", Cyberry_Potter_Status.Ir_Learn_Slot);
			}

			if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD_LONG){
				// 长按清空当前位槽
				IR_Raw_Signal_t empty = {0};
				IR_Flash_Save(Cyberry_Potter_Status.Ir_Learn_Slot, &empty);
				printf("Clear slot: %d\n", Cyberry_Potter_Status.Ir_Learn_Slot);
				Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
			}
			else if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD){
				Cyberry_Potter_Status.Ir_Learn_Slot++;
				if(Cyberry_Potter_Status.Ir_Learn_Slot >= IR_GESTURE_COUNT){
					Cyberry_Potter_Status.Ir_Learn_Slot = 0;
				}
				GPIO_Write(IR_LED_PORT,
					(GPIO_ReadOutputData(IR_LED_PORT) & ~IR_LED_MASK) |
					(Cyberry_Potter_Status.Ir_Learn_Slot << 3));
				printf("Learn slot: %d\n", Cyberry_Potter_Status.Ir_Learn_Slot);
				Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
			}
		}
		else{
			if(Cyberry_Potter_Status.Button_Status == BUTTON_HOLD && Cyberry_Potter_Status.IMU_Status == IMU_Idle){
				IMU_Sample_Start();
			}
		}

		if(Cyberry_Potter_Status.Button_Status != BUTTON_IDLE && Cyberry_Potter_Status.Button_Status != BUTTON_HOLD_VERY_LONG){
			Cyberry_Potter_Status.Button_Status = BUTTON_IDLE;
		}

		if(Cyberry_Potter_Status.IMU_Status == IMU_Sampled){
			LED_ON;
			#ifdef SYSTEM_MODE_DATA_COLLECT
			Delay_ms(200);
			IMU_Data_Print();
			#endif

			#ifndef SYSTEM_MODE_DATA_COLLECT
			model_feed_data();
			model_run(model);
			model_output = model_get_output();
			printf("%d",model_output);
			Model_LED(model_output);

			// RAW 发射
			if(model_output >= 0 && model_output < IR_GESTURE_COUNT){
				IR_Raw_Signal_t raw_sig;
				if(IR_Flash_Load((uint8_t)model_output, &raw_sig)){
					printf("IR: RAW replay %d edges\n", raw_sig.edge_count);
					IR_Send_Raw(&raw_sig);
				}
			}
			#endif
			Cyberry_Potter_Status.IMU_Status = IMU_Idle;
			EXTI_Restore();
		}


	}
}
