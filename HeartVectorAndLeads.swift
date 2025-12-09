import Foundation

// MARK: - Cardiac Vector

struct HeartVector {
    let x: Double
    let y: Double
    let z: Double

    static let zero = HeartVector(x: 0, y: 0, z: 0)

    func dot(_ other: HeartVector) -> Double {
        x * other.x + y * other.y + z * other.z
    }

    func scaled(by k: Double) -> HeartVector {
        HeartVector(x: x * k, y: y * k, z: z * k)
    }

    func normalized() -> HeartVector {
        let mag = sqrt(x*x + y*y + z*z)
        guard mag > 0 else { return self }
        return HeartVector(x: x/mag, y: y/mag, z: z/mag)
    }
}

// Vector addition
func + (a: HeartVector, b: HeartVector) -> HeartVector {
    HeartVector(x: a.x + b.x, y: a.y + b.y, z: a.z + b.z)
}

// MARK: - Standard ECG Leads

struct StandardLeads {

    // Helper for frontal-plane leads
    static func frontal(degrees: Double) -> HeartVector {
        let r = degrees * .pi / 180
        return HeartVector(
            x: cos(r),
            y: sin(r),
            z: 0
        ).normalized()
    }

    // Limb leads (Hexaxial Reference System)
    static let I   = frontal(degrees: 0)
    static let II  = frontal(degrees: 60)
    static let III = frontal(degrees: 120)

    static let aVR = frontal(degrees: -150)
    static let aVL = frontal(degrees: -30)
    static let aVF = frontal(degrees: 90)

    // Precordial (Chest) leads
    // +Z anterior, +X left lateral
    static let V1 = HeartVector(x: -0.6, y: 0.0, z:  0.8).normalized()
    static let V2 = HeartVector(x: -0.3, y: 0.0, z:  0.95).normalized()
    static let V3 = HeartVector(x:  0.0, y: 0.0, z:  1.0).normalized()
    static let V4 = HeartVector(x:  0.3, y: 0.0, z:  0.95).normalized()
    static let V5 = HeartVector(x:  0.6, y: 0.0, z:  0.8).normalized()
    static let V6 = HeartVector(x:  0.9, y: 0.0, z:  0.4).normalized()
}
