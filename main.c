  //1) Decimal -> IEEE (2) follow these steps: sign, exponent, significand
  //2) IEEE -> Decimal(2) follow these steps:
  //3) Quit
  #include <stdio.h>
  #include <math.h> //pow(base,exp) log(x)....

  /*************************/
  void dec2ieee()
  {
    /* declare local variables */
    float dec_number, orig_number;
    int dec_sign;
    int dec_exp = 0;
    int ieee_number;
    int biased_exp;
    int k;
    int mant_bit;
    int signif;
    int sign;
    /* prompt for floating point decimal number */
    printf("Enter the decimal representation: ");
    scanf("%f", &orig_number);
    dec_number = orig_number;


    /* Check for 0--if so, print result and return */   // 0 -> sign = 0, exponent = 00000000, significand = 0...0  IEEE-HEX: 00000000
    if(dec_number == 0.0)
    {
      printf("Sign: 0");
      printf("\nBiased exponent: 00000000");
      printf("\nMantissa: 00000000000000000000000 ");
      printf("\nIEEE-754 format: 00000000\n");
    
      return; 
    }

    /* Print sign: if number>0, sign is 0, else 1 */
    if(dec_number > 0.0)
    {
      dec_sign = 0;
    }
    else{
      dec_sign = 1;
      dec_number = 0.0 - dec_number;
    }
    printf("Sign: %d\n",dec_sign);

    /* take absolute value of number before generating significand */
    // dec_number = abs(dec_number); //may work or may not work

    /* Normalize number:
    while number >2, divide by 2, increment exponent*/
    while(dec_number >= 2){
      dec_number = dec_number/2;
      dec_exp++;
    }
    /*while number <1, multiply by 2, decrement exponent*/
    while(dec_number < 1)
    {
      dec_number = dec_number/2;
      dec_exp++;
    }

    /*Hint: there is a quicker way using logarithms with base 2*/ //log_a b = (log b)/(log a)
    //dec_exp = floor(log(dec_exp) / log(2)); //may or may not work due to compiler
    // dec_number = dec_number / pow(2, dec_exp); //can use this //pow is just 2 to the something)
    // dec_number = dec_number << dec_exp; // or this
      /* Bias exponent by 127 and print each bit in binary with 8-iteration for-loop*/
  printf("Biased exponent: ");  
      dec_exp += 127;
      biased_exp = dec_exp;
      for(k = 1; k <= 8; k++){
        if(dec_exp >= 128){
          printf("1");
          mant_bit = 1;
          dec_exp -= 128;
        }
        else{
          printf("0");
        }
        dec_exp *= 2;
        //signif = signif * 2 + mant_bit; //maybe it is here?
      }
  printf("\nMantissa: ");


      /* Hide 1 and print significand in binary with 23-iteration for-loop*/
      dec_number -= 1;
      //...
      for(k = 1; k <= 23; k++){
        if(dec_number >= 0.5){
          printf("1");
          dec_number -= 0.5;
          mant_bit = 1;
        }
        else{
          printf("0");
          mant_bit = 0;
        }
        dec_number *= 2;
        signif = signif * 2 + mant_bit;
        //printf("\nmant_bit: %d\n",mant_bit);
      }
    // signif = 0x00200000;
      
  
  printf("\n");
    /* Print IEEE-754 representation */// sign    exponent     significand
    //                                     __   _biased-exp_  _____________
    //                                   2^31
    //                                          ^ 32-bit # ^
    ieee_number =  (sign * pow(2, 31) + (biased_exp* pow(2,23)) + signif);

    printf("IEEE-754 format: %x\n",ieee_number); //%x is HEX int //may or may 
    //printf("IEEE-754 format: %x\n",ieee_number); //%x is HEX int //may or may not work
    return;
  }

  /***********************************************************************/
  void ieee2dec()
  {
    /* declare local variables */
    int ieee_number,ieee_exp, ieee_sign;
    float ieee_mant, decimal_number;
    /* prompt for IEEE-754 representation */
    printf("Enter the IEEE-754 representation: ");
    scanf("%x", &ieee_number);
    /* check for special cases: 0, -0, +infinity, -infinity, NaN,
      if so, print and return */
    if (ieee_number == 0x00000000){
      printf("Special case: +0\n");
      return;
    }
    if (ieee_number == 0x80000000)
    {
      printf("Special case: -0\n");
      return;
    }
    if(ieee_number == 0x7F800000)
    {
      printf("Special case: +inf\n");
      return;
    }
    if(ieee_number == 0xFF800000)
    {
      printf("Special case: -inf\n");
      return;
    }
    //... CHECK ALL SPECIAL CASES
    if((ieee_number & 0x7FFFFFFF) > 0x7F800000){ //NaN case
      printf("Special case: NaN\n");
      return;
    }
    /* Mask sign from number: if sign=0, print "+", else print "-" */
    ieee_sign = floor((ieee_number) / pow(2, 31));
    if(ieee_sign == 0)
    {
      printf("Sign: +");
    }
    else
    {
      printf("Sign: -");
    }
    printf("\n");
    /* Mask biased exponent and significand from number */
    ieee_exp = (ieee_number & 0x7f800000) / pow(2, 23);
    ieee_mant = (ieee_number & 0x007FFFFF) / pow(2, 23);
    /* If biased exponent=0, number is denormalized with unbiased exponent of -126,
      print denormalized number as fraction * 2^(-126), return */
    if(ieee_exp == 0){
      ieee_exp = -126;
      printf("Unbiased exponent: %d",ieee_exp);
      //ieee_number = ieee_number * pow(2,ieee_exp); //maybe ieee_number?
      if(ieee_sign == 0){
      printf("\nDenormalized decimal format: %f * (2^%d)\n",ieee_mant,ieee_exp);
      }
      else{
      printf("\nDenormalized decimal format: -%f * (2^%d)\n",ieee_mant,ieee_exp);
      }
      //printf("Unnormalized decimal number: %d")
    }
    /* Unbias exponent by subtracting 127 and print */
    else{
    ieee_exp = ieee_exp - 127; //i think its ieee_exp
    printf("Unbiased exponent: %d",ieee_exp);
      /* Add hidden 1 and print normalized decimal number */
    ieee_mant = (ieee_mant + 1); //I think its ieee_mant, beware: factor in the sign, (-1)^exp is -          (1-2*sign) is +
    printf("\nNormalized decimal: %f",ieee_mant);
    /* Print unnormalized decimal number */
    decimal_number = ieee_mant * pow(2,ieee_exp);
    printf("\nDecimal format: %f\n",decimal_number);

    }
    
  

    return;

  }

  int main()
  {
      /* declare local var's */
    int choice = 0;
    /* until user chooses to quit, print menu,
    select choice via switch statement and call appropriate function*/
    while (choice != 3)
    {
      printf("Measuring Performance:\n----------------------\n1) Decimal to IEEE-754 conversion\n2) IEEE-754 to Decimal conversion\n3) Exit program\nEnter selection : ");

      //...
      scanf("%d", &choice);
      switch (choice) {
      case 1: dec2ieee(); break;
      case 2: ieee2dec(); break;
      case 3: break;
      default: printf("Invalid choice.\nPlease choose from the following menu\n");
      } /* switch */

    } /* end-while */
    return 0;
  }
