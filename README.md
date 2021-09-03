## Linux 西洋棋 - 程式執行結果與說明 

###### 作者: 邱士懿

#### HW3 更新

將History的資料結構改成doubly linked list，使用此結構實現undo, redo和replay的功能，此次更改的內容有History.h, History.c和game.c裡的processReplay，詳情請見History.c中的註解。

#### A. 執行

在專案prebuild_executable資料夾中已經有編譯好的執行檔chess可以使用，若不想自行編譯整個專案，可以到此資料夾中(一定要先cd到此資料夾中再執行，否則儲存和載入檔案會發生問題)直接執行此檔案即可遊玩西洋棋。若想要編譯此專案的話，請參照B說明。

> <strong> 在執行之前請先將視窗拉大，否則畫面顯示會出問題 </strong>

#### B. 編譯
此專案有使用第三方套件ncurses，所以在編譯之前請先安裝ncurses，安裝指令為:
```<tr><tr>
sudo apt install libncurses5-dev
```
此專案有寫makefile，請使用make編譯。請到專案資料夾，在command line 上輸入make即可編譯整個專案。編譯完成後會出現名稱build的資料夾，裡面有執行檔chess，chess就是我們編譯完成的西洋棋程式。到build資料夾中執行chess，或是直接在專案資料夾輸入make run，便可開始遊玩。

#### C. 執行結果與說明
1. 遊戲一開始進去會出現開始畫面(圖一)，當中有操作輸入規則，請先閱讀好輸入規則再開始遊戲，準備好按enter就可以開始遊玩。輸入規則程式中有說明這邊不再贅述。
    <table>
        <tr style="border: none;">
            <td style="border: none; font-size: 12px" align="center">圖一 遊戲開始畫面</td>
        </tr>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; display: flex; flex-direction: column; align-items: center">
                <img src="https://i.imgur.com/rp3Qprp.png" width="230" >
            	<img src="https://i.imgur.com/qgmKvHa.png" width="230" >
            </td>
        </tr>
    </table>
    
    
    
2. 圖二為主要遊戲畫面，上方為計時器，中間為棋盤，下方為玩家輸入的地方。如果玩家輸入符合規則就會執行對應的操作，如果不符合輸入規則就會跳出錯誤提示，讓玩家重新輸入。
    <table>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; font-size: 12px" align="center">圖二 主要遊戲畫面</td>
        </tr>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; display: flex; flex-direction: column; align-items: center">
                <img src="https://i.imgur.com/z2JAPTb.png" width="250" >
            </td>
        </tr>
    </table>
    

    * 移動棋子: 輸入某棋子的位置以及要移動到的位置。以圖三為例，假如白色玩家想將knight從b1移到c3，輸入b1 c3後，換黑色玩家出手時，棋盤上的狀態就會顯示此棋已經從b1移到c3(圖四)。
        <table>
            <tr style="border: none; background-color: transparent">
                <td style="border: none; font-size: 12px" align="center">圖三</td>
        	<td style="border: none; font-size: 12px" align="center">圖四</td>
            </tr>
            <tr style="border: none; background-color: transparent">
            	<td style="border: none; font-size: 12px" align="center"><img src="https://i.imgur.com/waH5tab.png" width="350"></td>
                <td style="border: none; font-size: 12px" align="center"><img src="https://i.imgur.com/K8oVzNo.png" width="350"></td>
            </tr>
        </table>
        
    * 存檔: 輸入save後，會提示玩家輸入儲存檔案的名稱(圖五)，輸入完後會將棋局資料儲存於檔案中，並將檔案存到save的資料夾中(圖六)，以供未來讀取。
        <table>
            <tr style="border: none; background-color: transparent">
                <td style="border: none; font-size: 12px" align="center">圖五</td>
            	<td style="border: none; font-size: 12px" align="center">圖六</td>
            </tr>
            <tr style="border: none; background-color: transparent">
            	<td style="border: none; font-size: 12px" align="center"><img src="https://i.imgur.com/CUvKAfC.png" width="350"></td>
                <td style="border: none; font-size: 12px" align="center"><img src="https://i.imgur.com/VNWK0oD.png" width="350"></td>
            </tr>
        </table>

    * 讀檔: 輸入load後，會跳到載入檔案的畫面(圖六)，畫面會陳列所有在 save 檔案夾裡面的儲存檔名稱，輸入想要載入檔案的編號後就會載入儲存於檔案中的棋局。若輸入錯誤便會繼續遊戲。
    <table>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; font-size: 12px" align="center">圖七 載入檔案畫面</td>
        </tr>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; display: flex; flex-direction: column; align-items: center">
                <img src="https://i.imgur.com/Bwc69Qf.png" width="400" >
            </td>
        </tr>
    </table>

    * 退出/重新: 輸入quit/restart後，會提示玩家是否確定退出/重新遊戲，如果確定，就會執行輸入命令。

    * 悔棋/恢復: 輸入undo後棋盤狀態會回復到上一手棋，輸入redo則會取消最新的undo。

    * 重播: 輸入replay，會進入重播模式(圖八)。重播模式會從開局棋盤開始顯示，按A/D看上一個/下一個棋盤狀態，按Q則退出重播模式回到遊戲。

    <table>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; font-size: 12px" align="center">圖八 重播模式</td>
        </tr>
        <tr style="border: none; background-color: transparent">
            <td style="border: none; display: flex; flex-direction: column; align-items: center">
                <img src="https://i.imgur.com/V0hdpRF.png" width="400" >
            </td>
        </tr>
    </table> 

	* 設定計時器: 輸入timer和想要計時的時間(圖九)，例如: timer 100.0，會將計時器改成100秒(圖十)。玩家必須在設定的時間內出手，否則時間到就會直接換另一個玩家出手。
	<table>
	    <tr style="border: none; background-color: transparent">
	        <td style="border: none; font-size: 12px" align="center">圖九 設定計時器</td>
	        <td style="border: none; font-size: 12px" align="center">圖十 設定後</td>
	    </tr>
	    <tr style="border: none; background-color: transparent">
	        <td style="border: none; font-size: 12px" align="center"><img src="https://i.imgur.com/vl10xkR.png" width="350"></td>
	        <td style="border: none; font-size: 12px" align="center"><img src="https://i.imgur.com/zNqdhrF.png" width="350"></td>
	    </tr>
	</table>
