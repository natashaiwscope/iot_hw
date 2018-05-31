public bool XmodemUploadFile(string path)
        {
            /* control signals */
            const byte SOH = 1;  // Start of Header
            const byte EOT = 4;  // End of Transmission
            const byte ACK = 6;  // Positive Acknowledgement
            const byte NAK = 21; // Negative Acknowledgement

            /* sizes */
            const byte dataSize = 128;

            /* THE PACKET: 132 bytes */
            /* header: 3 bytes */
            // SOH
            int packetNumber = 0;
            int invertedPacketNumber = 255;
            /* data: 128 bytes */
            byte[] data = new byte[dataSize];
            /* footer: 1 byte */
            int checkSum = 0;

            /* get the file */
            FileStream fileStream = new FileStream(@path, FileMode.Open, FileAccess.Read);
                
            try
            {                
                /* get NAK */
                if (serialPort.ReadByte() != NAK) 
                {
                    Console.WriteLine("Can't start the transfer");
                    return false;
                }

                /* send packets with a cycle until we send the last byte */
                int fileReadCount;
                do
                {
                    /* if this is the last packet fill the remaining bytes with 0 */
                    fileReadCount = fileStream.Read(data, 0, dataSize);
                    if (fileReadCount == 0) break;
                    if (fileReadCount != dataSize)
                        for (int i = fileReadCount; i < dataSize; i++)
                            data[i] = 0;

                    /* calculate packetNumber */
                    packetNumber++;
                    if (packetNumber > 255)
                        packetNumber -= 256;
                    Console.WriteLine(packetNumber);

                    /* calculate invertedPacketNumber */
                    invertedPacketNumber = 255 - packetNumber;

                    /* calculate checkSum */
                    checkSum = 1;
                    checkSum += packetNumber;
                    checkSum += invertedPacketNumber;
                    for (int i = 0; i < dataSize; i++)
                        checkSum += data[i];

                    /* send the packet */
                    serialPort.Write(new byte[] { SOH }, 0, 1);
                    serialPort.Write(new byte[] { (byte)packetNumber }, 0, 1);
                    serialPort.Write(new byte[] { (byte)invertedPacketNumber }, 0, 1);
                    serialPort.Write(data, 0, dataSize);
                    serialPort.Write(new byte[] { (byte)checkSum }, 0, 1);

                    /* wait for ACK */
                    if (serialPort.ReadByte() != ACK)
                    {
                        Console.WriteLine("Couldn't send a packet.");
                        return false;
                    }
                } while (dataSize == fileReadCount);

                /* send EOT (tell the downloader we are finished) */
                serialPort.Write(new byte[] { EOT }, 0, 1);
                /* get ACK (downloader acknowledge the EOT) */
                if (serialPort.ReadByte() != ACK)
                {
                    Console.WriteLine("Can't complete the transfer.");
                    return false;
                }