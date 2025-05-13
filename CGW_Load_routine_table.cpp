#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include <filesystem>
#include <windows.h>
#include "seedNKeyDiag.h"
#include "CGW_cfg.h"
#include <unordered_map>
#include <unordered_set>  // Для unordered_set
#include <chrono>
#include <thread>
#include "TSMaster_modified.h"
using namespace std;
struct ChannelData {
	int channel;
	int rx;
	int expected_rx;
	int tx;
	int expected_tx;

	// Constructor to initialize all fields
	ChannelData(int ch, int r, int er, int t, int et)
		: channel(ch), rx(r), expected_rx(er), tx(t), expected_tx(et) {
	}
};
void prepareCanFrames(std::vector<TLIBCAN>& canFrames, const std::vector<uint32_t>& canIDs) {
	canFrames.clear();
	for (size_t i = 0; i < canIDs.size(); ++i) {
		TLIBCAN frame;
		frame.init_w_ext_id(static_cast<s32>(canIDs[i]), 8); // Initialize with extended ID
		frame.FIdxChn = static_cast<u8>(i);
		canFrames.push_back(frame);
	}
}
void __stdcall TSMasterLogger(const char* AStr, const s32 ALevel)
{
	std::cout << "TSMaster Logger: " << ALevel << ": " << AStr << "\n";
}




bool CheckOK(const int ACode)
{
	char* desc;
	vErrorCode = ACode;
	if (0 == ACode) return true;
	if (0 == tsapp_get_error_description(ACode, &desc)) {
		std::cout << "API error: " << desc << "\n";
	}
	else {
		std::cout << "API error code: " << ACode << "\n";
	}
	return false;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	if (initialize_lib_tsmaster("TSMasterTest\n") == 0) {
		cout << "initialize success\n";
	}
	else {
		cout << "initialize failed\n";
		return 0;
	}
	if (tsapp_set_can_channel_count(CGW.CAN_CH_count) == 0) {
		cout << "set channel success\n";
	}
	else {
		cout << "set channel failed\n";
		return 0;
	}
	tsapp_show_tsmaster_window("Channel Selection", true);
	tsapp_show_hardware_configuration_window();
	if (tsapp_connect() == 0) {
		cout << "connect successful\n";
	}
	else {
		cout << "connect failed\n";
		return 0;
	}
	if (tsdiag_can_create(CGW.Diagnostic_Module_Handle, CGW.CH_Index, CGW.CANFD_Support, CGW.Max_DLC, CGW.RequestID, CGW.RequestID_is_Std, CGW.ResponseID, CGW.ResponseID_is_Std, CGW.FunctionID, CGW.FunctionID_is_Std) == 0) {
		cout << "diag module create success\n";
	}
	else {
		cout << "diag module create failed\n";
		return 0;
	}


	if (tsapp_connect() == 0) {
		cout << "connect success\n";
	}
	else {
		cout << "connect failed\n";
		return 0;
	}
	if (tsdiag_set_p2_timeout(*CGW.Diagnostic_Module_Handle, CGW_UDS_timing.p2_timeout) == 0) {
		cout << "set timing success\n";
	}
	else {
		cout << "set timing failed\n";
		return 0;
	}
	if (tsdiag_set_p2_extended(*CGW.Diagnostic_Module_Handle, CGW_UDS_timing.p2_extended) == 0) {
		cout << "set timing success\n";
	}
	else {
		cout << "set timing failed\n";
		return 0;
	}
	if (tsdiag_set_s3_servertime(*CGW.Diagnostic_Module_Handle, CGW_UDS_timing.s3_servertime) == 0) {
		cout << "set timing success\n";
	}
	else {
		cout << "set timing failed\n";
		return 0;
	}
	if (tsdiag_set_s3_clienttime(*CGW.Diagnostic_Module_Handle, CGW_UDS_timing.s3_clienttime) == 0) {
		cout << "set timing success\n";
	}
	else {
		cout << "set timing failed\n";
		return 0;
	}
	tsdiag_set_filled_byte(*CGW.Diagnostic_Module_Handle, 0xCC);
	objDelay.delay(1);
	if (tstp_can_send_functional(*CGW.Diagnostic_Module_Handle, CGW_UDS_message.ext_session, CGW.ArraySize) == 0)
	{
		cout << "send diagnostic payload Success!\n";
	}
	else {
		cout << "send diagnostic payload Failed!\n";
		return 0;
	}
	objDelay.delay(1);
	if (tsdiag_can_security_access_request_seed(*CGW.Diagnostic_Module_Handle, seed_key.seed_level_ext, seed_key.seed_array_pointer, &seed_key.seed_array_size) == 0) {
		GenerateKeyEx_EXT(seed_key.seed_array_pointer,          /* Массив для seed [вход] */
			seed_key.seed_array_size,               /* Длина массива для seed [вход] */
			seed_key.security_level,         /* Уровень безопасности [вход] */
			seed_key.key_variant_pointer,                     /* Имя активного варианта [вход] */
			seed_key.key_array,                /* Массив для ключа [вход, выход] */
			seed_key.key_array_size,             /* Максимальная длина массива для ключа [вход] */
			seed_key.key_array_calculated_size);        /* Длина ключа [выход] */
	}
	else {
		cout << "request seed failed\n";
		return 0;
	}
	if (tsdiag_can_security_access_send_key(*CGW.Diagnostic_Module_Handle, seed_key.key_level_ext, seed_key.key_array_pointer, seed_key.key_array_size) == 0) {
		cout << "accesskey success\n";
	}
	else {
		cout << "accesskey failed\n";
		return 0;
	}
	objDelay.delay(1);
	unsigned short table_crc16 = Crc16(CGW_UDS_message.writeData);
	CGW_UDS_message.writeData[3000] = table_crc16 & 0xFF;
	CGW_UDS_message.writeData[3001] = (table_crc16 >> 8) & 0xFF;
	objDelay.delay(1);
	int AHandle = 0;
	tsfifo_enable_receive_fifo();
	TLIBCAN canframe;
	canframe.init_w_ext_id(0x18DA21F1, 8);
	canframe.is_tx = true;
	canframe.FIdxChn = 5;
	canframe.FData[0] = 0x1B;
	canframe.FData[1] = 0xC1;
	canframe.FData[2] = 0x2E;
	canframe.FData[3] = 0xA0;
	canframe.FData[4] = 0x01;
	canframe.FData[5] = 0x00;
	canframe.FData[6] = 0x00;
	canframe.FData[7] = 0x00;
	if (tsapp_transmit_can_async(&canframe) == 0) {
		cout << "send success\n";
	}
	else {
		cout << "send failed\n";
	}
	objDelay.delay_ms(5);
	//// Предположим, что CGW_UDS_message.routinetable_matrix — это двумерный массив
	int rows = 429;
	int cols = 8;
	TLIBCAN msg1;
	msg1.init_w_ext_id(0x18DA21F1, 8);
	msg1.is_tx = true;
	msg1.FIdxChn = 5;
	int count = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j += 8) {
			// Заполняем массив FData по 8 значений
			for (int k = 0; k < 8; k++) {
				if (j + k < cols) {
					msg1.FData[k] = CGW_UDS_message.routinetable_matrix[i][j + k];
				}
				else {
					msg1.FData[k] = 0x00; // Если данных не хватает, заполняем нулями
				}
			}

			// Передаем сообщение
			count += 1;
			if (tsapp_transmit_can_async(&msg1) == 0) {
				
			}
			else {
				cout << "load failed\n";
			}
			objDelay.delay_ms(10);
		}
	}
	objDelay.delay_ms(100);
	tsdiag_routine_control(*CGW.Diagnostic_Module_Handle, 0x01, 0x0200);
	objDelay.delay_ms(100);
	tsdiag_routine_control(*CGW.Diagnostic_Module_Handle, 0x03, 0x0200);
	if (tstp_can_request_and_get_response(*CGW.Diagnostic_Module_Handle, CGW_UDS_message.reqDataArray, CGW.ArraySize, CGW_UDS_message.responseArray, &CGW_UDS_message.responseArraySize) == 0)
	{
		cout << "Перезагрузка шлюза прошла успешно, маршруты записаны в шлюз\n";
	}
	else {
		cout << "Ошибка: выполнить перезагрузку шлюза не удалось\n";
		return 0;
	}
	printf("маршрутов: %d\n", count);
	cout << "Загрузка маршрутов в шлюз завершена\n";
	tsfifo_disable_receive_fifo();
	tsfifo_enable_receive_fifo();
	tsfifo_enable_receive_error_frames();
	std::vector<uint32_t> canIDs = {
				0x18E0441E, 0x18D05027, 0x18F01D13,
				0x18FF1C28, 0x18DAF91C, 0x18DAF98A
	};
	std::vector<TLIBCAN> canFrames;
	prepareCanFrames(canFrames, canIDs);
	tsapp_disconnect();
	finalize_lib_tsmaster();
	return 0;
}
