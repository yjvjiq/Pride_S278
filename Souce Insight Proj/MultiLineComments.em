macro MultiLineComment()

{

    hwnd = GetCurrentWnd()

    selection = GetWndSel(hwnd)

    LnFirst =GetWndSelLnFirst(hwnd)      //È¡Ê×ÐÐÐÐºÅ

    LnLast =GetWndSelLnLast(hwnd)      //È¡Ä©ÐÐÐÐºÅ

    hbuf = GetCurrentBuf()

 

    if(GetBufLine(hbuf, 0) =="//magic-number:tph85666031"){

        stop

    }

 

    Ln = Lnfirst

    buf = GetBufLine(hbuf, Ln)

    len = strlen(buf)

 

    while(Ln <= Lnlast) {

        buf = GetBufLine(hbuf, Ln)  //È¡Ln¶ÔÓ¦µÄÐÐ

        if(buf ==""){                   //Ìø¹ý¿ÕÐÐ

            Ln = Ln + 1

            continue

        }

 

        if(StrMid(buf, 0, 1) == "/"){       //ÐèÒªÈ¡Ïû×¢ÊÍ,·ÀÖ¹Ö»ÓÐµ¥×Ö·ûµÄÐÐ

            if(StrMid(buf, 1, 2) == "/"){

                PutBufLine(hbuf, Ln, StrMid(buf, 2, Strlen(buf)))

            }

        }

 

        if(StrMid(buf,0,1) !="/"){          //ÐèÒªÌí¼Ó×¢ÊÍ

            PutBufLine(hbuf, Ln, Cat("//", buf))

        }

        Ln = Ln + 1

    }

 

    SetWndSel(hwnd, selection)

}


macro AddMacroComment()

{

    hwnd=GetCurrentWnd()

    sel=GetWndSel(hwnd)

    lnFirst=GetWndSelLnFirst(hwnd)

    lnLast=GetWndSelLnLast(hwnd)

    hbuf=GetCurrentBuf()

 

    if (LnFirst == 0) {

            szIfStart = ""

    } else {

            szIfStart = GetBufLine(hbuf, LnFirst-1)

    }

    szIfEnd = GetBufLine(hbuf, lnLast+1)

    if (szIfStart == "#if 0" && szIfEnd =="#endif") {

            DelBufLine(hbuf, lnLast+1)

            DelBufLine(hbuf, lnFirst-1)

            sel.lnFirst = sel.lnFirst â€“ 1

            sel.lnLast = sel.lnLast â€“ 1

    } else {

            InsBufLine(hbuf, lnFirst, "#if 0")

            InsBufLine(hbuf, lnLast+2, "#endif")

            sel.lnFirst = sel.lnFirst + 1

            sel.lnLast = sel.lnLast + 1

    }

 

    SetWndSel( hwnd, sel )

}


