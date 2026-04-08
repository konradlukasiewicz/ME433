import time
import board
import pwmio

servo_pwm = pwmio.PWMOut(board.GP15, frequency=50, duty_cycle=0)

def setServo(angle):
    if angle < 0:
        angle = 0
    if angle > 180:
        angle = 180

    duty_fraction = 0.015 + (angle / 180.0) * 0.105
    servo_pwm.duty_cycle = int(duty_fraction * 65535)

while True:
    for i in range(10, 170, 10):
        setServo(i)
        time.sleep(0.1)

    for i in range(170, 10, -10):
        setServo(i)
        time.sleep(0.1)