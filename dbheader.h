#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>


	struct dbhead {//dbf headerje
		uint8_t version;
		uint8_t updTime[3];
		uint32_t recordNum;//ennyi adatnyi sor van �sszesen
		uint16_t byteNum;//ennyi byte van a headerben
		uint16_t headerbytes;//ennyi byte van egy sorban
		uint8_t zeros[2];
		uint8_t indiFlag;
		uint8_t encFlag;
		uint8_t reserved[12];
		uint8_t prodFlag;
		uint8_t langID;
		uint8_t nulla[2];
	};

	struct Fda {//field descriptor array tov�bbiakban csak fda
		uint8_t FName[11];
		uint8_t fieldType;
		uint8_t reserved[4];
		uint8_t length;
		uint8_t decimalCount;
		uint8_t areaID[2];
		uint8_t example;
		uint8_t res[10];
		uint8_t MDXflag;
	};

	class dbheader {
	private:
		std::ifstream ifs;
		dbhead Head_st; //dbhead struct t�pus� v�ltoz�, ide ker�l a dbf Header r�sze
		std::vector<Fda> fieldDesc; //Fda struct t�pus� vector, ide j�nnek a fda-k. Vector mivel nem tudjuk mennyi j�n �sszesen
		int largestfield;
	public:
		dbheader(const std::string &);

		void write(const std::string &);
	};
