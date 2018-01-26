#ifndef FLOOD_OUT
#define FLOOD_OUT
namespace oput {
	extern unsigned yoff, xoff, width, height, colors;
	void start();
	void end();
	void setup(unsigned w,unsigned h,unsigned c);
	void draw_scan(unsigned a,unsigned b,int color);
	void draw_board(char* data);
	void delay();
	int get_input();
	void ps(char* s);
}
#endif
