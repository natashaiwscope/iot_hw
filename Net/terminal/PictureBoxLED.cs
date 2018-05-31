// Port of bitmapped LED control to .NET, Windows Forms and C#
// by Robert Hinrichs (bobh@inav.net)
//
// Original Copyright notice for the MFC version
///////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998-1999 Monte Variakojis ( monte@apollocom.com )
// All rights reserved - not to be sold.
///////////////////////////////////////////////////////////////////////////

using System;
using System.Drawing;
using System.Windows.Forms;

// this is the only bitmapped LED class currently implemented
using PictureBoxLED16x16;

namespace PictureBoxLEDS
{
	/// <summary>
	/// This is an abstract layer between the client and the actual bitmapped
	/// device. It combines the Win Forms PictureBox control with the LED 
	/// control and was ment to decouple the PictureBox from the
	/// actual 16x16 bitmap implementation. Instances of other (larger)
	/// bitmaps would be made here. The client would know about the 
	/// implementations for the constructor but could use the
	/// IBitmapLED interface for the rest of the application.
	/// 
	/// PictureBoxLED inherits from a PictureBox control and implements
	/// the abstract IBitmapLED interface contract which all LEDs would agree
	/// to support (On()/Off()/etc). That interface could (should?) be in its 
	/// own file or here but it can be found currently in BitmapLED.cs where
	/// the implementations are.
	/// 
	/// In C++, this class would use multiple inheritance to achieve the same
	/// goal.
	/// 
	/// </summary>
	class PictureBoxLED : PictureBox , IBitmapLED
	{
		// this maps to PictureBox created visually on the design form
		private PictureBox thePbox;
		private LED16x16 theLED;
		private int m_nLedMode;

		// minimum constructor, create default
		public PictureBoxLED(System.Windows.Forms.Control parent,
			                 PictureBox pbox, Bitmap image)
		{
			thePbox = pbox;
			base.Parent = parent;
			theLED = new LED16x16(image);
			theLED.m_nLedColor = LED16x16.LED_COLOR.RED;
			theLED.m_nLedMode  = LED16x16.LED_STATE.OFF;
			theLED.m_nLedShape = LED16x16.LED_TYPE.ROUND;
			theLED.BlinkRate   = 0.0f;
			// register the handler with the LED so it can be called
			// when the LED is re-drawn. Must register before .LedDraw() since
			// that fires the event.
			theLED.LedChanged += new ChangedLEDEventHandler(BlinkDemoHandler);
			if(theLED.ledimage == null)
				pbox.Dispose();
			else
				theLED.LedDraw();

		}

		/// <summary>
		/// The completely specified constructor
		/// </summary>
		/// <param name="parent"></param>
		/// A Parent to be assigned to the control
		/// <param name="pbox"></param>
		/// PictureBox Control
		/// <param name="image"></param>
		/// The BitMapped file or resource
		/// <param name="color"></param>
		/// LED_COLOR.RED
		/// LED_COLOR.GREEN
		/// LED_COLOR.YELLOW	
		/// LED_COLOR.BLUE
		/// <param name="state"></param>
		/// LED_STATE.ON
		/// LED_STATE.OFF
		/// LED_STATE.DISABLED
		/// <param name="type"></param>
		/// LED_TYPE.ROUND
		/// LED_TYPE.SQUARE
		public PictureBoxLED(System.Windows.Forms.Control parent, PictureBox pbox, 
			Bitmap image,
			LED16x16.LED_COLOR color,
			LED16x16.LED_STATE state,
			LED16x16.LED_TYPE type)
		{
			thePbox = pbox;

			base.Parent = parent;

			theLED = new LED16x16(image);
			theLED.m_nLedColor = color;
			theLED.m_nLedMode  = state;
			theLED.m_nLedShape = type;
			theLED.BlinkRate   = 0.0f;
			// register the handler with the LED so it can be called
			// when the LED is re-drawn
			// Must register before .LedDraw() since
			// that fires the event.
			theLED.LedChanged += new ChangedLEDEventHandler(BlinkDemoHandler);
			if(theLED.ledimage == null)
				pbox.Dispose();
			else
				theLED.LedDraw();

		}

        /// <summary>
        /// The IBitmapLED interface decoupling
        /// </summary>
		// Implementation for IBitmapLED event
		public event ChangedLEDEventHandler LedChanged;

		public void BlinkDemoHandler(object obj, LEDEventArgs e)
		{
				PictureBoxLED16x16.LED16x16 bm = (PictureBoxLED16x16.LED16x16) obj;
				// copy the current LED image to the picturebox
				thePbox.Image = (Image)bm.displaybitmap;
				Rectangle rect = new Rectangle(0,0,bm.ledimage.Width,bm.ledimage.Height);
				Invalidate(rect);
		}

		public void LedDraw()
		{
			theLED.LedDraw();
		}
		public void LedOn()
		{
			theLED.LedOn();
		}
		public void LedDisable()
		{
			theLED.LedDisable();
		}
		public void LedOff()
		{
			theLED.LedOff();
		}
		public int  LedState
		{
			get {return (int)theLED.m_nLedMode;}
			set {m_nLedMode = (int)value;}
		}
		public float  BlinkRate
		{
			get {return theLED.m_nLedBlinkRate;}
			set {theLED.m_nLedBlinkRate = value;}
		}
		public void BlinkEnable()
		{
			theLED.BlinkEnable();
		}

		public void BlinkDisable()
		{
			theLED.BlinkDisable();
		}
		
	}// end PictureBoxLED class
}
