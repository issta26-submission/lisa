#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json[] = "{\"data\": [{\"val\": 123}, {\"raw\": 42}], \"meta\": {\"ok\": true}}";
    const char *endptr = NULL;
    size_t len = sizeof(json) - 1;
    cJSON *root = NULL;
    cJSON *data_arr = NULL;
    cJSON *meta = NULL;
    cJSON *ok_item = NULL;
    cJSON *extra_arr = NULL;
    cJSON *raw_item = NULL;
    cJSON *dup_raw = NULL;
    int data_count = 0;
    cJSON_bool ok_was_true = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, len, &endptr, 1);
    data_arr = cJSON_GetObjectItem(root, "data");
    data_count = cJSON_GetArraySize(data_arr);
    meta = cJSON_GetObjectItem(root, "meta");
    ok_item = cJSON_GetObjectItem(meta, "ok");
    ok_was_true = cJSON_IsTrue(ok_item);
    extra_arr = cJSON_AddArrayToObject(root, "extra");
    raw_item = cJSON_CreateRaw("unparsed:123");
    cJSON_AddItemToArray(extra_arr, raw_item);

    // step 3: Operate / Validate
    dup_raw = cJSON_Duplicate(raw_item, 1);
    is_raw = cJSON_IsRaw(raw_item);
    cmp = cJSON_Compare(raw_item, dup_raw, 1);
    (void)data_count;
    (void)ok_was_true;
    (void)is_raw;
    (void)cmp;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_raw);
    // API sequence test completed successfully
    return 66;
}