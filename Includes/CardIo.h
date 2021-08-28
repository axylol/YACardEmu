#ifndef CARDIO_H
#define CARDIO_H

#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>

class CardIo
{
public:
	enum StatusCode {
		Okay,
		SyncError,
		ChecksumError,
		EmptyResponseError,
		ServerWaitingReply,
	};

	CardIo();
	CardIo::StatusCode BuildPacket(std::vector<uint8_t> &buffer);
	CardIo::StatusCode ReceivePacket(std::vector<uint8_t> &buffer);
private:
	const uint8_t START_OF_TEXT = 0x02;
	const uint8_t END_OF_TEXT = 0x03;
	const uint8_t ENQUIRY = 0x05;
	const uint8_t ACK = 0x06;

	const uint8_t CARD_SIZE = 0x45;

	// RPS is really 3 bitmasks

	// R: 
	enum class ResultStatus {
		UNK_30 = 0x30,
		UNK_31 = 0x31,
		UNK_32 = 0x32,
		UNK_33 = 0x33,
		UNK_34 = 0x34,
	};

	enum class MechanismStatus {
		UNK_30 = 0x30,
		UNK_31 = 0x31,
		UNK_32 = 0x32,
		UNK_33 = 0x33,
		UNK_34 = 0x34,
		UNK_35 = 0x35,
		UNK_3A = 0x3A,
		JAM = 0x3B, // We know this is a jam from WMMT
	};








	enum class ReaderStatus {
		NoCard = 0x30,
		HasCard = 0x31,
		UNK_32 = 0x32,
		UNK_33 = 0x33,
		UNK_34 = 0x34,
	};

	enum class PrinterStatus {
		Idle = 0x30,
		Print = 0x31,
		UNK_32 = 0x32,
		UNK_33 = 0x33,
		UNK_34 = 0x34,
		UNK_35 = 0x35,
		UNK_3A = 0x3A,
		JAM = 0x3B,
	};

	enum class UNK_Status {
		Idle = 0x30,
		UNK_32 = 0x32, // Used during LoadCard, assumption is - reading.
		UNK_33 = 0x33,
		UKN_34 = 0x34, // Only seen in ReadCard if we don't have a card?
		UNK_35 = 0x35,
	};

	typedef struct {
		ReaderStatus Reader = ReaderStatus::NoCard;
		PrinterStatus Printer = PrinterStatus::Idle;
		UNK_Status Status = UNK_Status::Idle;

		void Reset() {
			Reader = ReaderStatus::NoCard;
			Printer = PrinterStatus::Idle;
			Status = UNK_Status::Idle;
		}
	} machine_status;

	machine_status RPS;

	uint8_t GetByte(uint8_t **buffer);
	void HandlePacket(std::vector<uint8_t> &packet);

	const std::string card_name = "test.bin";
	std::vector<uint8_t>card_data{};

	void LoadCardFromFS(std::string card_name);
	void SaveCardToFS(std::string card_name);

	void PutStatusInBuffer();

	// Commands
	int WMMT_Command_10_Init();
	int WMMT_Command_20_GetStatus();
	int WMMT_Command_33_Read();
	int WMMT_Command_40_Cancel();
	int WMMT_Command_53_Write(); // Write the mag strip
	int WMMT_Command_78_PrintSetting();
	int WMMT_Command_7A_ExtraCharacter();
	int WMMT_Command_7C_String(); // Write the text on the card
	int WMMT_Command_7D_Erase();
	int WMMT_Command_80_Eject();
	int WMMT_Command_A0_Clean(); // Clean print head
	int WMMT_Command_B0_GetCard();
	//int WMMT_Command_F5_BatteryCheck(); // Not used in WMMT2

	std::vector<uint8_t> ResponseBuffer{}; // Command Response
	std::vector<uint8_t> ProcessedPacket{};
};

#endif
