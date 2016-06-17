/*
 * process(grb_a, rgb_b, index, command)
 *
 * processes two frames
 *
 * rgb_a     = the first frame  ( array[640*480*3] )
 * rgb_b     = the second frame ( array[640*480*3] )
 * index       = an index to the frame number in a sequence of up to 16384 frames
 * command     = a specific operation to be applied on the frame
 *
 */

 
unsigned char tempframe[640*480*3];

int process(unsigned char *rgb_a, unsigned char *rgb_b,unsigned int index, unsigned char command) {


    unsigned int x,y;

	switch(command) {
		case 0 : // do nothing
			 break;		
		case 1 : // copy a to b
                for (y=0;y<480;y++) {
                  for (x=0;x<640;x++) {
                    rgb_b[(x*480+y)*3+REDV  ] = rgb_a[(x*480+y)*3+REDV  ];
                    rgb_b[(x*480+y)*3+GREENV] = rgb_a[(x*480+y)*3+GREENV];
                    rgb_b[(x*480+y)*3+BLUEV ] = rgb_a[(x*480+y)*3+BLUEV ];
                  }
                }
                break;
        case 2 : // diff with b and store in a
                for (y=0;y<480;y++) {
                  for (x=0;x<640;x++) {
                    rgb_a[(x*480+y)*3+REDV  ] = abs(rgb_a[(x*480+y)*3+REDV  ] - rgb_b[(x*480+y)*3+REDV  ] );
                    rgb_a[(x*480+y)*3+GREENV] = abs(rgb_a[(x*480+y)*3+GREENV] - rgb_b[(x*480+y)*3+GREENV] );
                    rgb_a[(x*480+y)*3+BLUEV ] = abs(rgb_a[(x*480+y)*3+BLUEV ] - rgb_b[(x*480+y)*3+BLUEV ] );
                  }
                }
                break;
        case 3 : // diff with b and store in a, store previous a in b
                 // copy a to tempframe
                for (y=0;y<480;y++) {
                  for (x=0;x<640;x++) {
                    tempframe[(x*480+y)*3+REDV  ] = rgb_a[(x*480+y)*3+REDV  ];
                    tempframe[(x*480+y)*3+GREENV] = rgb_a[(x*480+y)*3+GREENV];
                    tempframe[(x*480+y)*3+BLUEV ] = rgb_a[(x*480+y)*3+BLUEV ];
                  }
                }
                // copy diff(a,b) to a
                for (y=0;y<480;y++) {
                  for (x=0;x<640;x++) {
                    rgb_a[(x*480+y)*3+REDV  ] = abs(rgb_a[(x*480+y)*3+REDV  ] - rgb_b[(x*480+y)*3+REDV  ] );
                    rgb_a[(x*480+y)*3+GREENV] = abs(rgb_a[(x*480+y)*3+GREENV] - rgb_b[(x*480+y)*3+GREENV] );
                    rgb_a[(x*480+y)*3+BLUEV ] = abs(rgb_a[(x*480+y)*3+BLUEV ] - rgb_b[(x*480+y)*3+BLUEV ] );
                  }
                }
                // copy tempframe to b
                for (y=0;y<480;y++) {
                  for (x=0;x<640;x++) {
                    rgb_b[(x*480+y)*3+REDV  ]  = tempframe[(x*480+y)*3+REDV  ];
                    rgb_b[(x*480+y)*3+GREENV]  = tempframe[(x*480+y)*3+GREENV];
                    rgb_b[(x*480+y)*3+BLUEV ]  = tempframe[(x*480+y)*3+BLUEV ];
                  }
                }
                break;       
 	}

	return 0;
}
