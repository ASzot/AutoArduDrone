#pragma once

namespace SerialHelper
{
	void Start();

	void Print(int i);
	void Print(char* str);
	void Print(const char* str);
	void Print(double d);
	void Println(int i);
	void Println(char* str);
	void Println(const char* str);
	void Println(double d);
	void Println();

	bool Avaliable();
	char GetInput();
}