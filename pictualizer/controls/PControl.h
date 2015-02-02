
class PControl
{
	public:
		PControl(int x, int y, int w, int h);

		void setX(int x);
		void setY(int y);
		virtual void setWidth(int w);
		virtual void setHeight(int h);

		int getX();
		int getY();
		int getWidth();
		int getHeight();

		void moveTo(int x, int y);
		virtual void resize(int w, int h);

		virtual void draw() = 0;

	private:
		int x;
		int y;
		int w;
		int h;
};