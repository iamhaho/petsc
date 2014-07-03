//Make an array. Each array element (matInfo[0]. etc) will have all of the information of the questions.
var matInfo = [];
var currentAsk = "0";//start at id=0. then 00 01, then 000 001 010 011 etc if splitting two every time.
var askedA0 = false;//a one-way flag to record if A0 was asked

var treeDetailed;//global variable. tree.js uses this variable to determine how the tree should be displayed

//  This function is run when the page is first visited
//-----------------------------------------------------
$(document).ready(function(){

    //reset the form
    formSet(currentAsk);

    //must define these parameters before setting default pcVal, see populatePcList() and listLogic.js!
    matInfo[-1] = {
        posdef:  false,
        symm:    false,
        logstruc:false,
        blocks:  0,
        matLevel:0,
        id:      "0"
    }

    //display matrix pic. manually add square braces the first time
    $("#matrixPic").html("<center>" + "\\(\\left[" + getMatrixTex("0") + "\\right]\\)" + "</center>");
    MathJax.Hub.Queue(["Typeset",MathJax.Hub]);

    addEventHandlers();
});