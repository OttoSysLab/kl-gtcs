#############################################################################
# Generated by PAGE version 5.4
#  in conjunction with Tcl version 8.6
#  Jul 30, 2020 07:38:00 PM CST  platform: Windows NT
set vTcl(timestamp) ""
if {![info exists vTcl(borrow)]} {
    tk_messageBox -title Error -message  "You must open project files from within PAGE."
    exit}


if {!$vTcl(borrow) && !$vTcl(template)} {

set vTcl(actual_gui_bg) #d9d9d9
set vTcl(actual_gui_fg) #000000
set vTcl(actual_gui_analog) #ececec
set vTcl(actual_gui_menu_analog) #ececec
set vTcl(actual_gui_menu_bg) #d9d9d9
set vTcl(actual_gui_menu_fg) #000000
set vTcl(complement_color) #d9d9d9
set vTcl(analog_color_p) #d9d9d9
set vTcl(analog_color_m) #ececec
set vTcl(active_fg) #000000
set vTcl(actual_gui_menu_active_bg)  #ececec
set vTcl(pr,menufgcolor) #000000
set vTcl(pr,menubgcolor) #d9d9d9
set vTcl(pr,menuanalogcolor) #ececec
set vTcl(pr,treehighlight) firebrick
set vTcl(pr,autoalias) 1
set vTcl(pr,relative_placement) 1
set vTcl(mode) Relative
}



    menu .pop62 \
        -activebackground $vTcl(analog_color_m) -activeforeground #000000 \
        -background $vTcl(pr,menubgcolor) -font TkMenuFont \
        -foreground $vTcl(pr,menufgcolor) -tearoff 1 
    vTcl:DefineAlias ".pop62" "Popupmenu1" vTcl:WidgetProc "" 1

proc vTclWindow.top44 {base} {
    global vTcl
    if {$base == ""} {
        set base .top44
    }
    if {[winfo exists $base]} {
        wm deiconify $base; return
    }
    set top $base
    ###################
    # CREATING WIDGETS
    ###################
    vTcl::widgets::core::toplevel::createCmd $top -class Toplevel \
        -menu "$top.m61" -background $vTcl(actual_gui_bg) 
    wm focusmodel $top passive
    wm geometry $top 600x800+455+162
    update
    # set in toplevel.wgt.
    global vTcl
    global img_list
    set vTcl(save,dflt,origin) 0
    wm maxsize $top 3844 1055
    wm minsize $top 148 1
    wm overrideredirect $top 0
    wm resizable $top 1 1
    wm deiconify $top
    wm title $top "New Toplevel"
    vTcl:DefineAlias "$top" "Toplevel1" vTcl:Toplevel:WidgetProc "" 1
    set vTcl(real_top) {}
    vTcl:withBusyCursor {
    labelframe $top.lab46 \
        -font TkDefaultFont -foreground black -text Labelframe \
        -background $vTcl(actual_gui_bg) -height 116 -width 550 
    vTcl:DefineAlias "$top.lab46" "Labelframe1" vTcl:WidgetProc "Toplevel1" 1
    frame $top.fra54 \
        -borderwidth 2 -relief groove -background $vTcl(actual_gui_bg) \
        -height 346 -width 555 
    vTcl:DefineAlias "$top.fra54" "Frame1" vTcl:WidgetProc "Toplevel1" 1
    spinbox $top.spi57 \
        -activebackground #f9f9f9 -background white -buttonbackground #d9d9d9 \
        -disabledforeground #a3a3a3 -font TkDefaultFont -foreground black \
        -from 1.0 -highlightbackground black -highlightcolor black \
        -increment 1.0 -insertbackground black -selectbackground blue \
        -selectforeground white -textvariable spinbox -to 100.0 
    vTcl:DefineAlias "$top.spi57" "Spinbox1" vTcl:WidgetProc "Toplevel1" 1
    label $top.lab59 \
        -background $vTcl(actual_gui_bg) -disabledforeground #a3a3a3 \
        -font TkDefaultFont -foreground $vTcl(actual_gui_fg) -text {Job ID :} 
    vTcl:DefineAlias "$top.lab59" "Label1" vTcl:WidgetProc "Toplevel1" 1
    button $top.but60 \
        -activebackground $vTcl(analog_color_m) -activeforeground #000000 \
        -background $vTcl(actual_gui_bg) -disabledforeground #a3a3a3 \
        -font TkDefaultFont -foreground $vTcl(actual_gui_fg) \
        -highlightbackground $vTcl(actual_gui_bg) -highlightcolor black \
        -pady 0 -text Button 
    vTcl:DefineAlias "$top.but60" "Button1" vTcl:WidgetProc "Toplevel1" 1
    menu $top.m61 \
        -activebackground $vTcl(analog_color_m) -activeforeground #000000 \
        -background $vTcl(pr,menubgcolor) -font TkMenuFont \
        -foreground $vTcl(pr,menufgcolor) -tearoff 0 
    button $top.but64 \
        -activebackground $vTcl(analog_color_m) -activeforeground #000000 \
        -background $vTcl(actual_gui_bg) -disabledforeground #a3a3a3 \
        -font TkDefaultFont -foreground $vTcl(actual_gui_fg) \
        -highlightbackground $vTcl(actual_gui_bg) -highlightcolor black \
        -pady 0 -text Button 
    vTcl:DefineAlias "$top.but64" "Button2" vTcl:WidgetProc "Toplevel1" 1
    ###################
    # SETTING GEOMETRY
    ###################
    place $top.lab46 \
        -in $top -x 0 -relx 0.033 -y 0 -rely 0.088 -width 0 -relwidth 0.917 \
        -height 0 -relheight 0.145 -anchor nw -bordermode ignore 
    place $top.fra54 \
        -in $top -x 0 -relx 0.033 -y 0 -rely 0.25 -width 0 -relwidth 0.925 \
        -height 0 -relheight 0.433 -anchor nw -bordermode ignore 
    place $top.spi57 \
        -in $top -x 0 -relx 0.183 -y 0 -rely 0.041 -width 0 -relwidth 0.21 \
        -height 0 -relheight 0.03 -anchor nw -bordermode ignore 
    place $top.lab59 \
        -in $top -x 0 -relx 0.05 -y 0 -rely 0.038 -width 0 -relwidth 0.12 \
        -height 0 -relheight 0.034 -anchor nw -bordermode ignore 
    place $top.but60 \
        -in $top -x 0 -relx 0.45 -y 0 -rely 0.038 -height 33 -relheight 0 \
        -anchor nw -bordermode ignore 
    place $top.but64 \
        -in $top -x 0 -relx 0.817 -y 0 -rely 0.038 -height 33 -relheight 0 \
        -anchor nw -bordermode ignore 
    } ;# end vTcl:withBusyCursor 

    vTcl:FireEvent $base <<Ready>>
}

set btop ""
if {$vTcl(borrow)} {
    set btop .bor[expr int([expr rand() * 100])]
    while {[lsearch $btop $vTcl(tops)] != -1} {
        set btop .bor[expr int([expr rand() * 100])]
    }
}
set vTcl(btop) $btop
Window show .
Window show .top44 $btop
if {$vTcl(borrow)} {
    $btop configure -background plum
}

