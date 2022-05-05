using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ContosoUniversity.Models
{
    public class Student
    {

        public int ID { get; set; }

        [Required]
        [StringLength(50)]
        [Display(Name = "Last Name")]
        public string LastName { get; set; }

        [Required]
        [StringLength(50, ErrorMessage = "First name cannot be longer than 50 characters.")]
        [Column("FirstName")]
        [Display(Name = "First Name")]
        public string FirstMidName { get; set; }

        [DataType(DataType.Date)]
        [DisplayFormat(DataFormatString = "{0:yyyy-MM-dd}", ApplyFormatInEditMode = true)]
        [Display(Name = "Enrollment Date")]
        public DateTime EnrollmentDate { get; set; }

        [DataType(DataType.Date)]
        //[DisplayFormat(DataFormatString = "{dd'.'MM'.'yyyy'T'HH:mm:ss}")] Date format test
        [Display(Name = "Create Date")]
        public DateTime CreateDate { get; set; }

        [Display(Name = "LDR 1")]
        public int LDR1 { get; set; }

        [Display(Name = "LDR 2")]
        public int LDR2 { get; set; }

        [Display(Name = "LDR 3")]
        public int LDR3 { get; set; }

        [Display(Name = "LDR 4")]
        public int LDR4 { get; set; }

        [Display(Name = "LDR 5")]
        public int LDR5 { get; set; }

        [Display(Name = "Temperature")]
        public float Temperature
        {
            get; set;
        }

        [Display(Name = "Temp In Celc")]
        public string TemperatureInCelc
        {
            get
            {
                return Temperature.ToString("N3") + "°C";
            }
        }

        [Display(Name = "Humidity")]
        public float Humidity
        {
            get; set;
        }

        [Display(Name = "Heat Index")]
        public float HeatIndex
        {
            get; set;
        }

        [Display(Name = "Air Pressure")]
        public float AirPressure
        {
            get; set;
        }

        [Display(Name = "Altitude")]
        public float Altitude
        {
            get; set;
        }

        [Display(Name = "Lux")]
        public float Lux
        {
            get; set;
        }





        [Display(Name = "Full Name")]
        public string FullName
        {
            get
            {
                return LastName + ", " + FirstMidName;
            }
        }

        public ICollection<Enrollment> Enrollments { get; set; }
    }
}