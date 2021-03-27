#pragma once
#include <cliext/list>

#define MIN(x, y) (((x) < (y))? (x) : (y))
#define MAX(x, y) (((x) > (y))? (x) : (y))
#define ABS(x) (((x) > 0)? (x) : -(x))

namespace CppCLRWinformsProjekt {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		ref struct Box {
			Box(int cg, float cx, float cy, float w, float h) {
				category = cg;
				centerX = cx;
				centerY = cy;
				width = w;
				height = h;
			}
			int category;
			float centerX;
			float centerY;
			float width;
			float height;
		};
		static const array<Color>^ colors = gcnew array<Color>{Color::Blue, Color::Red, Color::Green, Color::Purple, Color::Brown, Color::Cyan, Color::Coral};
		bool drawing = false;
		bool loaded = false;
		int img_index = 0;
		int category_now = 0;
		Drawing::Image^ image;
		String^ text_file;
		array<String^>^ files;
		cliext::list<Box^>^ boxes;
		Drawing::Pen^ pen, ^ axis_pen;
		Point view_position, cursor_now, box_corner;

		Form1(void)
		{
			image = nullptr;
			text_file = nullptr;
			files = nullptr;
			boxes = gcnew cliext::list<Box^>();
			pen = gcnew Drawing::Pen(Color::Black, 2.0f);
			axis_pen = gcnew Drawing::Pen(Color::Gray, 1.0f);
			view_position = Point(0, 0);
			cursor_now = Point(0, 0);
			box_corner = Point(0, 0);
			InitializeComponent();
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	protected:
	private: System::Windows::Forms::ToolStripMenuItem^ importToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::FolderBrowserDialog^ folderBrowserDialog1;
	protected:
	private: System::ComponentModel::IContainer^ components;

	private:


#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->importToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->importToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(582, 28);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// importToolStripMenuItem
			// 
			this->importToolStripMenuItem->Name = L"importToolStripMenuItem";
			this->importToolStripMenuItem->Size = System::Drawing::Size(68, 24);
			this->importToolStripMenuItem->Text = L"Import";
			this->importToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::importToolStripMenuItem_Click);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->pictureBox1->Location = System::Drawing::Point(0, 28);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(582, 526);
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_Paint);
			this->pictureBox1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseDown);
			this->pictureBox1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseMove);
			this->pictureBox1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseUp);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(582, 553);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"Annotator";
			this->ResizeEnd += gcnew System::EventHandler(this, &Form1::Form1_ResizeEnd);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void importToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (folderBrowserDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			files = System::IO::Directory::GetFiles(folderBrowserDialog1->SelectedPath, "*.jpg");
			MessageBox::Show(Convert::ToString(files->Length) + " files have read");
			if (files->Length > 0) {
				image = Image::FromFile(files[0]);
				text_file = files[0]->Substring(0, files[0]->LastIndexOf(".")) + ".txt";

				if (System::IO::File::Exists(text_file)) {
					System::IO::StreamReader^ din = System::IO::File::OpenText(text_file);
					String^ str;
					while ((str = din->ReadLine()) != nullptr) {
						array<String^>^ args = str->Split(' ');
						boxes->push_back(gcnew Box(
							Convert::ToInt32(args[0]),
							Convert::ToDouble(args[1]) * image->Width,
							Convert::ToDouble(args[2]) * image->Height,
							Convert::ToDouble(args[3]) * image->Width,
							Convert::ToDouble(args[4]) * image->Height)
						);
					}
					din->Close();
				}
				pictureBox1->Invalidate();
				loaded = true;
			}
		}
	}
	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		if (loaded) {
			if (files != nullptr && files->Length > 0)
				e->Graphics->DrawImage(image, view_position.X, view_position.Y, image->Width, image->Height);

			e->Graphics->DrawLine(axis_pen, Point(0, cursor_now.Y), Point(pictureBox1->Width, cursor_now.Y));
			e->Graphics->DrawLine(axis_pen, Point(cursor_now.X, 0), Point(cursor_now.X, pictureBox1->Height));

			if (drawing) {
				pen->Color = colors[category_now];
				e->Graphics->DrawRectangle(pen, Drawing::Rectangle(Point(MIN(box_corner.X, cursor_now.X), MIN(box_corner.Y, cursor_now.Y)), Drawing::Size(ABS(box_corner.X - cursor_now.X), ABS(box_corner.Y - cursor_now.Y))));
			}

			for each (Box ^ b in boxes) {
				pen->Color = colors[b->category];
				e->Graphics->DrawRectangle(pen, Drawing::Rectangle(b->centerX - b->width * 0.5f + view_position.X, b->centerY - b->height * 0.5f + view_position.Y, b->width, b->height));
			}
			pen->Color = Color::Black;

			Drawing::Font^ tickFont = gcnew Drawing::Font("Times New Roman", 12.0f);
			e->Graphics->DrawString("(" + Convert::ToString(cursor_now.X - view_position.X) + ", " + Convert::ToString(cursor_now.Y - view_position.Y) + ", " + Convert::ToString(category_now) + ")", tickFont, Brushes::Black, RectangleF(0, 0, 150, 50));
		}
	}
	private: System::Void Form1_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (loaded) {
			switch (e->KeyCode) {
			case Keys::W: view_position.Y += 50; break;
			case Keys::A: view_position.X += 50; break;
			case Keys::S: view_position.Y -= 50; break;
			case Keys::D: view_position.X -= 50; break;
			case Keys::Z:
				boxes->pop_back();
				break;
			case Keys::Q:
				if (img_index > 0) {
					image = Image::FromFile(files[--img_index]);
					category_now = 0;
					boxes->clear();

					text_file = files[img_index]->Substring(0, files[img_index]->LastIndexOf(".")) + ".txt";

					if (System::IO::File::Exists(text_file)) {
						System::IO::StreamReader^ din = System::IO::File::OpenText(text_file);
						String^ str;
						while ((str = din->ReadLine()) != nullptr) {
							array<String^>^ args = str->Split(' ');
							boxes->push_back(gcnew Box(
								Convert::ToInt32(args[0]),
								Convert::ToDouble(args[1]) * image->Width,
								Convert::ToDouble(args[2]) * image->Height,
								Convert::ToDouble(args[3]) * image->Width,
								Convert::ToDouble(args[4]) * image->Height)
							);
						}
						din->Close();
					}
				}
				else {
					MessageBox::Show("You have reach the beginning of the images");
				}
				break;
			case Keys::E:
				if (img_index < files->Length - 1) {
					image = Image::FromFile(files[++img_index]);
					category_now = 0;
					boxes->clear();

					text_file = files[img_index]->Substring(0, files[img_index]->LastIndexOf(".")) + ".txt";

					if (System::IO::File::Exists(text_file)) {
						System::IO::StreamReader^ din = System::IO::File::OpenText(text_file);
						String^ str;
						while ((str = din->ReadLine()) != nullptr) {
							array<String^>^ args = str->Split(' ');
							boxes->push_back(gcnew Box(
								Convert::ToInt32(args[0]),
								Convert::ToDouble(args[1]) * image->Width,
								Convert::ToDouble(args[2]) * image->Height,
								Convert::ToDouble(args[3]) * image->Width,
								Convert::ToDouble(args[4]) * image->Height)
							);
						}
						din->Close();
					}
				}
				else {
					MessageBox::Show("You have reach the end of the images");
				}
				break;
			case Keys::R: category_now = (category_now + 1) % 4; break;
			case Keys::Space:
				IO::StreamWriter^ sw = gcnew IO::StreamWriter(text_file);
				for each (Box ^ b in boxes)
					sw->WriteLine("{0} {1} {2} {3} {4}", b->category, b->centerX / image->Width, b->centerY / image->Height, b->width / image->Width, b->height / image->Height);
				sw->Close();

				if (img_index < files->Length - 1) {
					image = Image::FromFile(files[++img_index]);
					category_now = 0;
					boxes->clear();

					text_file = files[img_index]->Substring(0, files[img_index]->LastIndexOf(".")) + ".txt";

					if (System::IO::File::Exists(text_file)) {
						System::IO::StreamReader^ din = System::IO::File::OpenText(text_file);
						String^ str;
						while ((str = din->ReadLine()) != nullptr) {
							array<String^>^ args = str->Split(' ');
							boxes->push_back(gcnew Box(
								Convert::ToInt32(args[0]),
								Convert::ToDouble(args[1]) * image->Width,
								Convert::ToDouble(args[2]) * image->Height,
								Convert::ToDouble(args[3]) * image->Width,
								Convert::ToDouble(args[4]) * image->Height)
							);
						}
						din->Close();
					}
				}
				else {
					MessageBox::Show("You have reach the end of the images");
				}
				break;
			}

			pictureBox1->Invalidate();
		}
	}
	private: System::Void Form1_ResizeEnd(System::Object^ sender, System::EventArgs^ e) {
		pictureBox1->Size = Drawing::Size(Form1::Width, Form1::Height);
	}
	private: System::Void pictureBox1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (loaded) {
			cursor_now = Point(MIN(MAX(e->Location.X, view_position.X), image->Width + view_position.X), MIN(MAX(e->Location.Y, view_position.Y), image->Height + view_position.Y));
			pictureBox1->Invalidate();
		}
	}
	private: System::Void pictureBox1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (loaded) {
			box_corner = Point(MAX(e->Location.X, view_position.X), MAX(e->Location.Y, view_position.Y));
			drawing = true;
		}
	}
	private: System::Void pictureBox1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (loaded) {
			drawing = false;
			float width = ABS(box_corner.X - cursor_now.X);
			float height = ABS(box_corner.Y - cursor_now.Y);
			float centerX = MIN(box_corner.X, cursor_now.X) + 0.5f * width - view_position.X;
			float centerY = MIN(box_corner.Y, cursor_now.Y) + 0.5f * height - view_position.Y;

			boxes->push_back(gcnew Box(category_now, centerX, centerY, width, height));
		}
	}
	};
}
