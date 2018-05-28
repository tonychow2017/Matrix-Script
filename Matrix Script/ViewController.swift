//
//  ViewController.swift
//  Matrix Script
//
//  Created by Chun Wa Chow on 14/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import UIKit

class ViewController: UIViewController, UITextViewDelegate {
    //MARK: Properties
    @IBOutlet weak var answerLabel: UILabel!
    @IBOutlet weak var mainTextView: UITextView!

    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        mainTextView.delegate = self
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
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
    
    func textViewDidChange(_ textView: UITextView) {
        let input = textView.text
        if let answerString = input?.withCString({ (ptr: UnsafePointer<Int8>) -> String in
            return calculateExpression(expression: UnsafeMutablePointer<Int8>(mutating: ptr))
        }) {
            answerLabel.text = answerString
        } else {
            answerLabel.text = "N/A"
        }
    }
}

