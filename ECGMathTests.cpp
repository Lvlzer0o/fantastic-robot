#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <vector>

#include "ECGMath.h"
#include "ECGMorphology.h"
#include "ECGSimulation.h"

TEST(HeartVectorMath, Addition)
{
    const Heart_vector a{1.0, 2.0, 3.0};
    const Heart_vector b{0.5, -1.0, 4.0};
    const Heart_vector result = add(a, b);

    EXPECT_DOUBLE_EQ(result.x, 1.5);
    EXPECT_DOUBLE_EQ(result.y, 1.0);
    EXPECT_DOUBLE_EQ(result.z, 7.0);
}

TEST(HeartVectorMath, ScalarMultiplication)
{
    const Heart_vector v{2.0, -3.0, 0.5};
    const Heart_vector result = scale(v, 2.5);

    EXPECT_DOUBLE_EQ(result.x, 5.0);
    EXPECT_DOUBLE_EQ(result.y, -7.5);
    EXPECT_DOUBLE_EQ(result.z, 1.25);
}

TEST(HeartVectorMath, LeadProjectionDotProduct)
{
    const Heart_vector heart_vector{1.0, 2.0, -1.0};
    const Heart_vector lead_vector{0.0, 0.5, 0.5};

    const float64 expected_dot = (heart_vector.x * lead_vector.x) + (heart_vector.y * lead_vector.y) + (heart_vector.z * lead_vector.z);
    const float64 projection = project_to_lead(heart_vector, lead_vector);

    EXPECT_DOUBLE_EQ(projection, expected_dot);
}

TEST(MorphologySafety, ComponentVectorIsFinite)
{
    Ecg_component component{};
    component.start_time_s = 0.0;
    component.duration_s = 0.5;
    component.is_active = true;
    component.shape_params.direction = normalize({0.25, 0.5, 0.25});
    component.shape_params.scale = 1.0;
    component.shape_params.center = 0.5;
    component.shape_params.width = 0.25;

    for (int32 step = 0; step <= 100; ++step)
    {
        const float64 t = static_cast<float64>(step) / 100.0;
        const Heart_vector result = calculate_component_vector(&component, t);
        EXPECT_TRUE(std::isfinite(result.x));
        EXPECT_TRUE(std::isfinite(result.y));
        EXPECT_TRUE(std::isfinite(result.z));
    }
}

TEST(ECGSimulation, GoldenNormalSinus60Bpm500Hz)
{
    const float64 pr_interval = 0.16;
    const float64 qrs_duration = 0.10;
    const float64 qrs_axis = 60.0;
    const float64 heart_rate_bpm = 60.0;
    const float64 sampling_rate_hz = 500.0;
    const float64 duration_seconds = 1.0;
    const float64 tolerance = 1e-6;

    const Ecg_morphology morphology = create_normal_sinus_morphology(pr_interval, qrs_duration, qrs_axis);
    const std::vector<Lead_sample> samples = generate_ecg_timeseries(morphology, heart_rate_bpm, sampling_rate_hz, duration_seconds);

    const std::array<std::array<double, 13>, 7> expected_samples = {
        std::array<double, 13>{0.00000000, 0.00137367, 0.00274735, 0.00137367, -0.00237927, 0.00000000, 0.00237927, -0.00082420, -0.00041366, 0.00000000, 0.00041366, 0.00082420, 0.00125528},
        std::array<double, 13>{0.10000000, 0.00137367, 0.00274735, 0.00137367, -0.00237927, 0.00000000, 0.00237927, -0.00082420, -0.00041366, 0.00000000, 0.00041366, 0.00082420, 0.00125528},
        std::array<double, 13>{0.20000000, 0.12576443, 0.25152885, 0.12576443, -0.21783038, 0.00000000, 0.21783038, -0.07545866, -0.03787161, 0.00000000, 0.03787161, 0.07545866, 0.11492499},
        std::array<double, 13>{0.30000000, 0.00111317, 0.00602387, 0.00491071, -0.00412057, -0.00219251, 0.00631308, -0.00066790, -0.00033521, 0.00000000, 0.00033521, 0.00066790, 0.00101722},
        std::array<double, 13>{0.40000000, 0.06077686, 0.32889242, 0.26811556, -0.22497566, -0.11970705, 0.34468271, -0.03646612, -0.01830182, 0.00000000, 0.01830182, 0.03646612, 0.05553860},
        std::array<double, 13>{0.50000000, 0.00111317, 0.00602387, 0.00491071, -0.00412057, -0.00219251, 0.00631308, -0.00066790, -0.00033521, 0.00000000, 0.00033521, 0.00066790, 0.00101722},
        std::array<double, 13>{1.00000000, 0.00137367, 0.00274735, 0.00137367, -0.00237927, 0.00000000, 0.00237927, -0.00082420, -0.00041366, 0.00000000, 0.00041366, 0.00082420, 0.00125528},
    };

    for (const auto& expected : expected_samples)
    {
        const std::size_t index = static_cast<std::size_t>(expected[0] * sampling_rate_hz);
        ASSERT_LT(index, samples.size());

        const Lead_sample& sample = samples.at(index);
        EXPECT_NEAR(sample.time_s, expected[0], tolerance);
        for (std::size_t lead = 0; lead < expected.size() - 1; ++lead)
        {
            EXPECT_NEAR(sample.leads.at(lead), expected[lead + 1], tolerance);
        }
    }
}
