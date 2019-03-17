use itertools::Itertools;
use proptest::prelude::*;
use qsort::qsort;

proptest! {
    #![proptest_config(
        ProptestConfig {
            timeout: 100,
            .. ProptestConfig::default()
        }
    )]

    #[test]
    #[ignore]
    fn sort(mut data: Vec<u8>) {
        let original = data.clone();
        qsort(&mut data[..]);
        prop_assert_eq!(original.len(), data.len());
        for val in &original {
            prop_assert!(data.contains(val));
        }
        for (prev, cur) in data.iter().tuple_windows() {
            prop_assert!(prev <= cur);
        }
    }
}
