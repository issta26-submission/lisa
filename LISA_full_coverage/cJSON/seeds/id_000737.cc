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
//<ID> 737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"title\":\"test\",\"numbers\":[1,2,3],\"meta\":{\"count\":3}}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithOpts(json_text, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *meta = cJSON_GetObjectItem(root, "meta");
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_SetNumberHelper(count_item, count_val + 1.0);
    const char *labels[] = {"one", "two", "three"};
    cJSON *label_arr = cJSON_CreateStringArray(labels, 3);
    cJSON_AddItemToObject(root, "labels", label_arr);

    // step 3: Operate
    cJSON *new_num = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(numbers, 1, new_num);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}