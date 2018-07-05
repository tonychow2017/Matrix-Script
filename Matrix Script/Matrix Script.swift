//
//  Matrix Script.swift
//  Matrix Script
//
//  Created by Chun Wa Chow on 22/6/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import Foundation

class MatrixScript {
    func calculateExpression(expression: UnsafeMutablePointer<Int8>) -> String {
        let resultString = UnsafeMutablePointer<Int8>(mutating: nil)
        let resultStringPointer = UnsafeMutablePointer<UnsafeMutablePointer<Int8>?>.allocate(capacity: 1)
        resultStringPointer.initialize(to: resultString)
        calculate_c(resultStringPointer, expression)
        return String(cString: resultStringPointer.pointee!)
    }

    //MARK: UITextViewDelegate
    //MARK: Actions

    /*@IBAction func buttonInput(_ sender: UIButton) {
     if let insertingString = buttonInsertString[sender] {
     mainTextView.text.append(insertingString)
     let input = mainTextView.text
     if let answerString = input?.withCString({ (ptr: UnsafePointer<Int8>) -> String in
     return calculateExpression(expression: UnsafeMutablePointer<Int8>(mutating: ptr))
     }) {
     answerLabel.text = answerString
     } else {
     answerLabel.text = "N/A"
     }
     }
     }*/

    func calculate(input: String) -> (String, String?) {
        let ans = input.withCString({ (ptr: UnsafePointer<Int8>) -> String in
            return calculateExpression(expression: UnsafeMutablePointer<Int8>(mutating: ptr))
        })
        if ans.starts(with: "Error: ") {
            return ("Error", String(ans.dropFirst(7)))
        } else {
            return (ans,nil)
        }
    }
}

