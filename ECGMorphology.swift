import Foundation

// MARK: - ECG Component (Vector-Based)

struct ECGComponent {
    let start: Double          // seconds from beat start
    let duration: Double       // seconds
    let shape: (Double) -> HeartVector   // phase [0,1] → vector

    func vector(at time: Double) -> HeartVector {
        let local = time - start
        guard local >= 0, local <= duration else {
            return .zero
        }
        let u = local / duration
        return shape(u)
    }
}

// MARK: - ECG Morphology

struct ECGMorphology {
    let pWave: ECGComponent?
    let qrs: ECGComponent
    let tWave: ECGComponent?

    func heartVector(at localTime: Double) -> HeartVector {
        var v = HeartVector.zero

        if let p = pWave {
            v = v + p.vector(at: localTime)
        }

        v = v + qrs.vector(at: localTime)

        if let t = tWave {
            v = v + t.vector(at: localTime)
        }

        return v
    }
}

// MARK: - Wave Shape Helpers

func gaussianEnvelope(u: Double, center: Double, width: Double) -> Double {
    exp(-pow((u - center) / width, 2))
}

func directionalShape(axisDegrees: Double, scale: Double) -> (Double) -> HeartVector {
    let r = axisDegrees * .pi / 180
    let dir = HeartVector(
        x: cos(r),
        y: sin(r),
        z: 0
    ).normalized()

    return { u in
        let mag = gaussianEnvelope(u: u, center: 0.5, width: 0.25)
        return dir.scaled(by: scale * mag)
    }
}

// MARK: - Normal Sinus Morphology (Corrected)

func normalSinusMorphology(
    prInterval: Double,
    qrsDuration: Double,
    qtInterval: Double,
    qrsAxisDegrees: Double = 60
) -> ECGMorphology {

    let pStart = 0.0
    let qrsStart = prInterval
    let tStart = qrsStart + qrsDuration + 0.04

    let pWave = ECGComponent(
        start: pStart,
        duration: 0.10,
        shape: directionalShape(axisDegrees: 60, scale: 0.15)
    )

    let qrs = ECGComponent(
        start: qrsStart,
        duration: qrsDuration,
        shape: directionalShape(axisDegrees: qrsAxisDegrees, scale: 1.2)
    )

    // T-wave is concordant with QRS (NOT inverted)
    let tWave = ECGComponent(
        start: tStart,
        duration: 0.20,
        shape: directionalShape(axisDegrees: qrsAxisDegrees + 20, scale: 0.35)
    )

    return ECGMorphology(
        pWave: pWave,
        qrs: qrs,
        tWave: tWave
    )
}
