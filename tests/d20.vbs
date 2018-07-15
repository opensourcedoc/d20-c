' Set Program States
Dim Program : Program = "d20.exe"
Dim DistDir : DistDir = "dist"

Result1D6 = Capture(DistDir & "\" & Program)(0)
Assert _
	(1 <= Result1D6) And (Result1D6 <= 6) , _
	"Wrong value"

Result1D8 = Capture(DistDir & "\" & Program & " -r 1 -d 8")(0)
Assert _
	(1 <= Result1D8) And (Result1D8 <= 8) , _
	"Wrong value"

Result2D6p1 = Capture(DistDir & "\" & Program & " -r 2 -d 6 -m 1")(0)
Assert _
	(3 <= Result2D6p1) And (Result2D6p1 <= 13) , _
	"Wrong value"

' Home-made assert for VBScript.
Sub Assert( boolExpr, strOnFail )
	If not boolExpr then
		Err.Raise vbObjectError + 99999, , strOnFail
	End If
End Sub

' Capture stdout and stderr from cmd
Function Capture(cmd)
	Set WshShell = WScript.CreateObject("WScript.Shell")

	Set output = WshShell.Exec("cmd.exe /c " & cmd)

	Dim arr(2)
	
	arr(0) = ""

	Do
		o = output.StdOut.ReadLine()
		arr(0) = arr(0) & o & vbNewLine
	Loop While Not output.Stdout.atEndOfStream
	
	arr(1) = ""
	
	Do
		e = output.StdErr.ReadLine()
		arr(1) = arr(1) & e & vbNewLine
	Loop While Not output.StdErr.atEndOfStream
	
	Capture = arr
End Function
