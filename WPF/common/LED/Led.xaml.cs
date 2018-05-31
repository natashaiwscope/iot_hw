/*
 * Written by  Michele Cattafesta
 * released with BSD License (you can use, modify, redistribute, sell and everything else you want to do) 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;

namespace LedControl
{
    /// <summary>
    /// Logica di interazione per UserControl1.xaml
    /// </summary>
    public partial class Led : UserControl
    {
        #region Dependency properties

        /// <summary>Dependency property to Get/Set the current IsActive (True/False)</summary>
        public static readonly DependencyProperty IsActiveProperty =
            DependencyProperty.Register("IsActive", typeof(bool), typeof(Led),
                new PropertyMetadata(false, new PropertyChangedCallback(Led.IsActivePropertyChanced)));

        /// <summary>Dependency property to Get/Set Color when IsActive is true</summary>
        public static readonly DependencyProperty ColorOnProperty =
            DependencyProperty.Register("ColorOn", typeof(Color), typeof(Led),
                new PropertyMetadata(Colors.Green,new PropertyChangedCallback(Led.OnColorOnPropertyChanged)));

        /// <summary>Dependency property to Get/Set Color when IsActive is false</summary>
        public static readonly DependencyProperty ColorOffProperty =
            DependencyProperty.Register("ColorOff", typeof(Color), typeof(Led),
                new PropertyMetadata(Colors.Red,new PropertyChangedCallback(Led.OnColorOffPropertyChanged)));

        /// <summary>Dependency property to Get/Set Color when IsActive is false</summary>
        public static readonly DependencyProperty ColorBusyProperty =
            DependencyProperty.Register("ColorBusy", typeof(Color), typeof(Led),
                new PropertyMetadata(Colors.Yellow, new PropertyChangedCallback(Led.OnColorBusyPropertyChanged)));

        /// <summary>Dependency property to Get/Set if led will flash</summary>
        public static readonly DependencyProperty FlashingProperty =
            DependencyProperty.Register("Flashing", typeof(bool), typeof(Led),
            new PropertyMetadata(false,new PropertyChangedCallback(Led.OnFlashingPropertyChanged)));

        /// <summary>Dependency property to Get/Set period of flash in milliseconds</summary>
        public static readonly DependencyProperty FlashingPeriodProperty =
            DependencyProperty.Register("FlashingPeriod", typeof(int), typeof(Led),
                new PropertyMetadata(500,new PropertyChangedCallback(Led.OnFlashingPeriodPropertyChanged)));

        #endregion 

        #region Wrapper Properties

        /// <summary>Gets/Sets Value</summary>
        public bool IsActive
        {
            get { return (bool)GetValue(IsActiveProperty); }
            set 
            {
                SetValue(IsActiveProperty, value);
            }
        }

        /// <summary>Gets/Sets Color when led is True</summary>
        public Color ColorOn
        {
            get
            {
                return (Color)GetValue(ColorOnProperty);
            }
            set
            {
                SetValue(ColorOnProperty, value);
            }
        }

        /// <summary>Gets/Sets Color when led is False</summary>
        public Color ColorOff
        {
            get
            {
                return (Color)GetValue(ColorOffProperty);
            }
            set
            {
                SetValue(ColorOffProperty, value);
            }
        }

        /// <summary>Gets/Sets Color when led is False</summary>
        public Color ColorBusy
        {
            get
            {
                return (Color)GetValue(ColorBusyProperty);
            }
            set
            {
                SetValue(ColorBusyProperty, value);
            }
        }

        /// <summary>Gets/Sets Flashing beaviour on true or false</summary>
        public bool Flashing
        {
            get
            {
                return (bool)GetValue(FlashingProperty);
            }
            set
            {
                SetValue(FlashingProperty, value);
            }
        }

        /// <summary>Gets/Sets Flashing period in ms</summary>
        public int FlashingPeriod
        {
            get
            {
                return (int)GetValue(FlashingPeriodProperty);
            }
            set
            {
                SetValue(FlashingPeriodProperty, value);
            }
        }

        #endregion 

        #region Private fields

        DispatcherTimer timer = new DispatcherTimer();        

        #endregion 

        #region Constructor

        public Led()
        {
            InitializeComponent();
            timer.Interval = TimeSpan.FromMilliseconds(FlashingPeriod);
            timer.Tick += new EventHandler(OnFlashingTimerElapsed);
            timer.IsEnabled = false;
            backgroundColor.Color = IsActive ? ColorOn : ColorOff;
        }

        #endregion
        
        #region Callbacks

        /// <summary> tick of flashing timer </summary>
        void OnFlashingTimerElapsed(object sender, EventArgs e)
        {
            if (backgroundColor.Color == ColorOn)
            {
                backgroundColor.Color = ColorBusy;
            }
            else
            {
                backgroundColor.Color = ColorOn;
            }
        }

        private static void OnFlashingPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e) 
        {
            Led led = (Led)d;
            if (led.IsActive)
            {
                //if led is not active do nothing
                if (led.Flashing)
                {
                    if(!led.timer.IsEnabled)
                    {
                        led.backgroundColor.Color = led.ColorBusy;
                        led.timer.IsEnabled       = true;
                        led.timer.Start();
                    }
                }
                else
                {
                    //flashing just stopped - color should be "on"
                    led.timer.Stop();
                    led.timer.IsEnabled       = false;
                    led.backgroundColor.Color = led.ColorOn;
                }   
            }
            else
            {
                led.timer.Stop();
                led.timer.IsEnabled       = false;
                led.backgroundColor.Color = led.ColorOff;
            }
        }

        private static void OnFlashingPeriodPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Led led = (Led)d;
            led.timer.Interval = TimeSpan.FromMilliseconds((int)e.NewValue);                
        }

        /**
         * Is active property chanced callback. If the led is no longer active, set color to null value 
         * and stop timer.
         *
         */
        private static void IsActivePropertyChanced(DependencyObject d, DependencyPropertyChangedEventArgs e) 
        {
            Led led = (Led)d;
            if (!led.IsActive)
            {
                //stop any running timer
                led.backgroundColor.Color = led.ColorOff;
                led.timer.Stop();
                led.timer.IsEnabled = false;
            }
            else
            {
                led.backgroundColor.Color = led.ColorOn;
            }

        }

        private static void OnColorOnPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Led led = (Led)d;
            led.ColorOn = (Color)e.NewValue;
            if (led.IsActive == true)
                led.backgroundColor.Color = led.ColorOn;
        }

        private static void OnColorOffPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Led led = (Led)d;
            led.ColorOff = (Color)e.NewValue;
            if (led.IsActive == false)
                led.backgroundColor.Color = led.ColorOff; 
        }

        private static void OnColorBusyPropertyChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            Led led = (Led)d;
            led.ColorBusy = (Color)e.NewValue;
            if (led.IsActive && led.Flashing)
            {
                led.backgroundColor.Color = led.ColorBusy;
            }
        }


        #endregion

    }
}
