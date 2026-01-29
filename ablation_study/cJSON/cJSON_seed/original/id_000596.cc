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
//<ID> 596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"user\": \"bob\", \"age\": 42, \"score\": 3.14 } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *age_item = cJSON_GetObjectItem(root, "age");
    double age_val = cJSON_GetNumberValue(age_item);
    double nums[4];
    nums[0] = 1.1;
    nums[1] = 2.2;
    nums[2] = age_val;
    nums[3] = 4.4;
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 4);
    cJSON_AddItemToObject(root, "double_array", dbl_arr);

    // step 3: Operate
    cJSON *score_item = cJSON_GetObjectItem(root, "score");
    double old_score = cJSON_GetNumberValue(score_item);
    cJSON *new_score = cJSON_CreateNumber(old_score * 2.0);
    cJSON_ReplaceItemViaPointer(root, score_item, new_score);
    cJSON *score_after = cJSON_GetObjectItem(root, "score");
    double new_score_val = cJSON_GetNumberValue(score_after);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    (void)age_val;
    (void)new_score_val;
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}