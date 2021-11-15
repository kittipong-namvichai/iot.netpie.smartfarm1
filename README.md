# iot.netpie.smartfarm1
1.สามารถศึกษาการใช้งาน NETPIE2020 จาก NECTEC ตามลิ้งด้านล่างนะครับ
  * Link การอบรมจาก NECTEC https://www.youtube.com/watch?v=aaKq2t-o3kw&list=PL9xicgqwnniqtI9ve5sXydZj-AdJtWyNb&index=10
  * Link เอกสารประกอบการอบรมจาก NECTEC https://bit.ly/2Wbrp6y

2. ต่อวงจรตาม ShematicSmartFarm.pdf หรือ เทียบเคียง

3. อุปกรณ์หลักที่ใช้ในการควบคุม (Controller List)
  * ESP32-WROOM ใช้ Source Code ESP32_PROJECT.ino
  * Arduino UNO ใช้ Source Code ARDUINO_PROJECT.ino

4. Arduino จะส่ง output ON เป็น Active Low ส่วน OFF จะเป็น Active High เพราะ Module Relay จะใช้ Common Vcc เป็น 5V
