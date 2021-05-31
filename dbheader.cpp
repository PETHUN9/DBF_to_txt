#include <algorithm>
#include "dbheader.h"


	dbheader::dbheader(const std::string &filename)
		:ifs(filename, std::ios::binary) {
		unsigned pedometer = 0;
		if (!ifs) {//ha nem sikerül megnyitni a fájlt hibát dob
			throw 1;
		}
		ifs.read(reinterpret_cast<char *>(&Head_st), sizeof(Head_st)); //beolvas egy Head_st méretû darabot a fájlból

		std::cout << Head_st.version << ", ";
		uint8_t v = Head_st.version & 7;
		bool dBaseForDos = (Head_st.version >> 3) & 1;
		uint8_t SQL = (Head_st.version >> 4) & 7;
		bool memo = (Head_st.version >> 7) & 1;
		for (int i = 0; i < 3; ++i)
			std::cout << static_cast<int>(Head_st.updTime[i]) << ", ";
		std::cout << Head_st.recordNum << ", ";
		std::cout << Head_st.byteNum << ", ";
		std::cout << Head_st.headerbytes << ", ";
		for (int i = 0; i < 2; ++i)
			std::cout << Head_st.zeros[i] << ", ";
		std::cout << Head_st.indiFlag << ", ";
		std::cout << Head_st.encFlag << ", ";
		for (int i = 0; i < 12; ++i)
			std::cout << Head_st.reserved[i] << ", ";
		std::cout << Head_st.prodFlag << ", ";
		std::cout << Head_st.langID << ", ";
		for (int i = 0; i < 2; ++i)
			std::cout << Head_st.nulla[i] << ", ";
		std::cout << std::endl;

		pedometer = 31; //lépésszámláló ami azért 31, mert a header 32 byte méretû (0-31)

		while (pedometer < Head_st.byteNum - 1) {
			pedometer += 32;//egy fda 32 byte méretû és egy ciklusban egy fda-t olvas be
			char end;
			ifs.read(&end, 1);//egy bit olvasása, ha header végét jelzõ bit, akkor kilép
			if (end == 0x0D)
				break;

			fieldDesc.push_back(Fda());
			Fda &record = fieldDesc.back();

			memcpy(&record, &end, 1);
			ifs.read(reinterpret_cast<char *>(&record) + 1, sizeof(Fda) - 1); //fda-1 méretet beolvas

			largestfield = std::max(largestfield, static_cast<int>(record.length));
		}
		std::cout << std::endl;
	}

	void dbheader::write(const std::string &FinalDest){
		std::ofstream out(FinalDest);
		std::vector<char> Buffer;
		Buffer.resize(largestfield);
		int totalbytes = 0;
		int recnum = 0;

		for (int k = 0; k < fieldDesc.size(); ++k) {//fda-k fájlba kiírásához szükséges ciklus
			for (int j = 0; j < 11; ++j) {
				char temp = static_cast <char>(fieldDesc[k].FName[j]);
				out.write(&temp, 1u);
			}
			out.write(",        ", 8u);
		}
		out.write("\n", 1u);

		while (!ifs.eof()) {
			char record;
			ifs.read(&record, 1u);
			if (record == 0x1A)//fájl végét jelzõ byte
				break;
			if (ifs.fail())
				break;
			int j = 0;
			int temphead = 0;
			while (temphead < Head_st.headerbytes - 1) {//egy sornyi adat olvasásához való ciklus
				char t[100];
				int i = 0;

				for (i; i < fieldDesc[j].length; i++) {//egy mezõnyi adat olvasásához való ciklus
					ifs.read(reinterpret_cast<char *>(&t[i]), 1u);
					++totalbytes;
				}

				for (int k = 0; k < fieldDesc[j].length; k++) {//egy mezõnyi adat kiírásához való ciklus
					out.write(&t[k], 1u);
				}
				temphead += fieldDesc[j].length;
				out.write(", ", 1u);
				j++;
			}
			++recnum;
			out.write("\n", 1u);
		}
		std::cout << " created " << recnum << " records and " << totalbytes << " bytes." << std::endl;//plusz info hány sor volt és összesen hány byte
	}
