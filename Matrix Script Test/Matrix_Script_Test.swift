//
//  Matrix_Script_Test.swift
//  Matrix Script Test
//
//  Created by Chun Wa Chow on 22/6/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

import XCTest
//@testable import Matrix_Script

class Matrix_Script_Test: XCTestCase {
    
    override func setUp() {
        super.setUp()
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }
    
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }
    
    func testExample() {
        // This is an example of a functional test case.
        // Use XCTAssert and related functions to verify your tests produce the correct results.
        XCTAssertEqual(MatrixScript().calculate(input: "1+2"), "3")
        XCTAssertEqual(MatrixScript().calculate(input: "1+2*3-5/10"), "6.5")
        XCTAssertEqual(MatrixScript().calculate(input: "[1,2+3]"), "[1, 5]")
        XCTAssertEqual(MatrixScript().calculate(input: "[2,3]+1"), "[3, 4]")
        XCTAssertEqual(MatrixScript().calculate(input: "cos(0)"), "1")
        XCTAssertEqual(MatrixScript().calculate(input: "factorial(0)"), "1")
        XCTAssertEqual(MatrixScript().calculate(input: "factorial(10)"), "3.6288e+06")
        XCTAssertEqual(MatrixScript().calculate(input: "factorial(abs([-3,-4]))"), "[6, 24]")
        XCTAssertEqual(MatrixScript().calculate(input: "range(1,2)"), "[1, 2]")
        XCTAssertEqual(MatrixScript().calculate(input: "range(1,4.5)"), "[1, 2, 3, 4]")
        XCTAssertEqual(MatrixScript().calculate(input: "range(3,-3)"), "[-3, -2, -1, 0, 1, 2, 3]")
        XCTAssertEqual(MatrixScript().calculate(input: "det([-1,-2,-3;4,5,6;-7,-8,-9])"), "0")
        XCTAssertEqual(MatrixScript().calculate(input: "max(range(10,100))"), "100")
        XCTAssertEqual(MatrixScript().calculate(input: "maxmin(abs(range(-100,100)))"), "[100, 0]")
        XCTAssertEqual(MatrixScript().calculate(input: "sum(3*range(1,10))"), "165")
        XCTAssertEqual(MatrixScript().calculate(input: "1/0"), "Error: Division by zero.")
        XCTAssertEqual(MatrixScript().calculate(input: "(12"), "Error: Mismatched bracket.")
        XCTAssertEqual(MatrixScript().calculate(input: "1+[2"), "Error: Unclosed matrix.")
        XCTAssertEqual(MatrixScript().calculate(input: "log(2,3,4)"), "Error: Wrong number of argument(s).")
        //XCTAssertEqual(MatrixScript().calculate(input: ""), "")
    }
    
    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }
    
}
