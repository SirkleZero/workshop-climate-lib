size_t strlen_F(const __FlashStringHelper *ifsh)
{
	PGM_P p = reinterpret_cast<PGM_P>(ifsh);
	size_t n = 0;
	while (1)
	{
		unsigned char c = pgm_read_byte(p++);
		if (c == 0) break;
		n++;
	}
	return n;
}

char *strcpy_F(const __FlashStringHelper *message)
{
	char tmp[128];
	String tmpStr = message;
	strncpy(tmp, tmpStr.c_str(), 128);
	return tmp;
}
