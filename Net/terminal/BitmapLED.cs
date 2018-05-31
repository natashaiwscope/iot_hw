using System;
using System.Drawing;
using System.ComponentModel;
using System.Windows.Forms;
using System.Collections;
using System.IO;
using System.Diagnostics;

using PictureBoxLEDS;

namespace PictureBoxLED16x16
{
	/// <summary>
	/// Summary description for PictureBoxLED16x16.
	/// Implements the 16x16 Bitmapped LED and the IBitmapLED
	/// interface for drawing on a PictureBox Control.
	/// 
	/// </summary>
	///
	// Port of bitmapped LED control to .NET, Windows Forms and C#
	// by Robert Hinrichs (bobh@inav.net)
	//
	// Original Copyright notice for the MFC version
	///////////////////////////////////////////////////////////////////////////
	// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
	// All rights reserved - not to be sold.
	///////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// A delegate type for connecting LED change notifications.
	/// The registered delegate will be called when the LED is
	/// re-drawn and the LedChanged event is fired.
	/// </summary>
	/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
	public delegate void ChangedLEDEventHandler(object sender, 
	                                            LEDEventArgs e );
	/*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

	////////////////////////////////////////////
	/// <summary>
	/// The IBitmapLED interface
	/// </summary>
	public interface IBitmapLED
	{
		void  LedDraw();
		void  LedOn();
		void  LedOff();
		void  LedDisable();
		int   LedState{get;
		               set;}
		float BlinkRate{get;
			            set;}
		void  BlinkEnable();
		void  BlinkDisable();
		// event fired when the LED is re-drawn
		event ChangedLEDEventHandler LedChanged;
	}// end BitmapLED interface
    ////////////////////////////////////////////
	/// <summary>
	/// The 16x16 BItmapped implementation
	/// </summary>
	
	// Custom event
	// Holds
	public class LEDEventArgs : EventArgs
	{ 
		private LED16x16.LED_STATE lstate;

		internal LEDEventArgs(LED16x16.LED_STATE ledstate)
		{
			lstate = ledstate;
		}
		internal LEDEventArgs()
		{
			lstate = LED16x16.LED_STATE.INVALID;
		}

		internal LED16x16.LED_STATE RetID()
		{
           return lstate;
		}

	}

	public class LED16x16 : IBitmapLED 
	{
		// could be readonly with other led sizes
		internal const int LED_SIZE = 16;

		internal enum LED_STATE 
		{
			ON       = 0,            // col 0
			OFF      = 1,            // col 1
			DISABLED = 2,            // col 2
			INVALID  = 3
		};
		internal enum LED_TYPE
		{ 
			ROUND  = 0, 
			SQUARE = 4 * LED_SIZE 
		};
		internal enum LED_COLOR
		{
			RED    = 0 * LED_SIZE,   // row 0
			GREEN  = 1 * LED_SIZE,   // row 1
			YELLOW = 2 * LED_SIZE,   // row 2
			BLUE   = 3 * LED_SIZE    // row 3
		};

        
		internal LED_COLOR m_nLedColor;
		internal LED_TYPE  m_nLedShape;
		internal LED_STATE m_nLedMode;

		private Timer timer = new Timer();
		internal float m_nLedBlinkRate;

		private Bitmap DisplayBitmap = new Bitmap(16,16);
		public  Bitmap displaybitmap
		{
			get{return DisplayBitmap;}
			set{DisplayBitmap = value;}
		}

		private Bitmap LedImage;
		public  Bitmap ledimage
		{
			get{return LedImage;}
			set{LedImage = value;}
		}

		private bool blinkenabled;

		public event ChangedLEDEventHandler LedChanged;

		public LED16x16(Bitmap bitmaps)
		{
			Color TransparentColor = Color.FromArgb(255,0,255);
			try
			{
				ledimage = bitmaps;
				Bitmap TempLED = ledimage;
				TempLED.MakeTransparent(TransparentColor);
				ledimage = TempLED;
			}
			catch
			{
				MessageBox.Show(
					@"Error: Bitmaps invalid ",
					"",
					MessageBoxButtons.OKCancel,
					MessageBoxIcon.Error);

				ledimage = null;
			}

			//			foreach(LED_COLOR ledcolor in Enum.GetValues(typeof (LED_COLOR)))
			//				Debug.WriteLine(ledcolor);
		}	

		public void LedDraw( )
		{
			
			// Compare this to the MFC version. Does it
			// have the same functionality?

			// Call on every change to the displayed image
			// Fires a delegate event.

			// draw on the bitmap to be displayed in OnPaint
			Graphics gImage = Graphics.FromImage(displaybitmap);
			Rectangle rect = new Rectangle((int)m_nLedMode*LED_SIZE,
				(int)m_nLedColor+(int)m_nLedShape,
				LED_SIZE,LED_SIZE);
			// draw from the larger image to the smaller
			gImage.DrawImage(ledimage,0,0,rect,GraphicsUnit.Pixel);
			gImage.Dispose();
			// cause an event, notify anyone interested
			// wrapper for LedChanged delegate event
			OnLedChanged(new LEDEventArgs(m_nLedMode)); // custom event arg
		}

		// Responsible method for notifying registered objects of the event
		// Can be overridden by derived class
		protected virtual void OnLedChanged( LEDEventArgs e )
		{
			LED_STATE CurrentState = e.RetID(); // custom event arg
			// These are the LED state changes we choose to update on
			if( (CurrentState == LED_STATE.ON)  | 
				(CurrentState == LED_STATE.OFF) |
				(CurrentState == LED_STATE.DISABLED) )
			{
				if(LedChanged != null)		// if anyone has signed up to be
					LedChanged(this, e);    //notified of an LED change
			}
		}

		/// <summary>
		/// IBitmapLED interface implenentation
		/// </summary>
		
		public void LedOn()
		{
			m_nLedMode  = LED_STATE.ON;
			LedDraw();
		}
		public void LedDisable()
		{
			m_nLedMode  = LED_STATE.DISABLED;
			LedDraw();
		}
		public void LedOff()
		{
			m_nLedMode  = LED_STATE.OFF;
			LedDraw();
		}
		public int  LedState
		{
			get {return (int)m_nLedMode;}
			set {m_nLedMode = (LED_STATE)value;}
		}
		public float  BlinkRate
		{
			get {return m_nLedBlinkRate;}
			set {m_nLedBlinkRate = value;}
		}

		public void BlinkEnable()
		{
			if(blinkenabled != true)
			{
				blinkenabled = true;
				BlinkRate = 1.0f;
				timer.Interval = (int)BlinkRate*1000;
				timer.Tick += new EventHandler(BlinkOnTick);
				timer.Start();
			}
		}

		public void BlinkEnable(float rate)
		{
			if(blinkenabled != true)
			{
				blinkenabled = true;
				BlinkRate = rate;
				timer.Interval = (int)rate*1000;
				timer.Tick += new EventHandler(BlinkOnTick);
				timer.Start();
			}
		}

		public void BlinkDisable()
		{
			if(blinkenabled == true)
			{
				blinkenabled = false;
				timer.Stop();
				timer.Tick -= new EventHandler(BlinkOnTick);
			}
		}

		public void BlinkOnTick(object obj, EventArgs e)
		{
			if(LedState==(int)LED_STATE.ON)
				LedState = (int)LED_STATE.OFF;
			else
				LedState = (int)LED_STATE.ON;

			LedDraw();
		}

	}// end LED16x16 Class

}// end PictureBoxLED16x16 namespace
