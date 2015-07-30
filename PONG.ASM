Title ASM Pong - by Brian Creasy and Kyle Lanser
.model small   ; Data segment < 64K, code segment < 64K
.stack 100h    ; Set up 256 bytes of stack space

.data

beepcode db 07h                         ; ascii code for bell
PaddleLeft dw 100                       ; current center of left paddle
PaddleRight dw 100                      ; current center of right paddle

ViewX dw 320                            ; maximum X coord
ViewY dw 200                            ; maximum Y coord

BallX dw 160                            ; current X coord of the ball
BallY dw 100                            ; current Y coord of the ball

DirX db 0                               ; 0 - left/up
DirY db 0                               ; 1 - right/down

.code

main proc
; set the data segment
  mov  ax, @data
  mov  ds, ax                           ; Point DS to the data segment

; set the video mode to 320x200
  mov  ah, 00h                          ; Set video mode
  mov  al, 13h                          ; Mode 13H
  int  10h

; set the video memory to 'es'
  mov  ax, 0a000h
  mov  es, ax                           ; Set ES to point to the VGA

MainLoop:
; main loop for animation

  call drawscreen
  call ballmove

  mov  ah, 01h                          ; check for a keypress
  int  16h                              ; is a key waiting in the buffer?
  jz   MainLoop                         ; no, loop right back to the start

  mov  ah, 00h                          ; yes, so get the key
  int  16h

  call quit
  ret
main endp

drawscreen proc
; calculates where the ball is and masks it into the buffer

  xor  dx, dx                           ; Y loop
  xor  cx, cx                           ; X loop
  xor  di, di

  mov  dx, 200                          ; 200 rows
  Y_Loop:                               ; begin Y loop
    mov  cx, 320                        ; 320 columns
    X_Loop:
      mov  bx, 0                        ; we need to set pixel color of the 
      cmp  dx, BallY                    ; current location of the ball to
      jne  draw                         ; white while leaving the rest of the
                                        ; screen black.
      cmp  cx, BallX                    ;  
      jne  draw                         ; 
                                        ;
      mov  bx, 7                        ;

      draw:
        call drawPaddle                 ; draw the paddles onto the screen

        mov  es:[di], bx                ; redraw the vga buffer

        inc  di                         ; next pixel
        dec  cx                         ; horizontal pixel left one
    jnz  X_Loop                         ; have we done all the columns?
    dec  dx
  jnz  Y_Loop                           ; have we done all the rows?
  ret
drawscreen endp

ballmove proc
; move the ball around the xy coordinate plane

  cmp  DirX, 1
  jne  goLeft
    inc  BallX
    jmp  endleft
  goLeft:
    dec  BallX
  endleft:

  cmp  DirY, 1
  jne  goUp
    inc  BallY
    jmp  endup
  goUp:
    dec  BallY
  endup:

  call checkbounds

  ret
ballmove endp

checkbounds proc
; make sure ball is still on the screen / aka: bounce

  cmp  BallX, 0
  jne  skip1
    mov  DirX, 1                        ; go right
  skip1:

  cmp  BallY, 0
  jne  skip2
    mov  DirY, 1                        ; go down
  skip2:

  mov  AX, ViewX
  cmp  AX, BallX 
  jne  skip3
    mov  DirX, 0                        ; go left
  skip3:

  mov  ax, ViewY
  cmp  ax, BallY
  jne  skip4
    mov  DirY, 0                        ; go up
  skip4:

  ret
checkbounds endp
                
beep proc
; procedure does not work. fixme.

  mov  dl, 07h
  mov  ax, 02h
  int  21h
  ret
beep endp

drawPaddle proc
; procedure under construction

  ret
drawPaddle endp

quit proc
; resets video mode and exits to system

  mov  ah, 00h                          ; set video mode
  mov  al, 03h                          ; mode 03h
  int  10h

  mov  ax, 4C00H                        ; return to DOS
  int  21h
  ret
quit endp

end main
