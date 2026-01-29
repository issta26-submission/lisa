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
//<ID> 563
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"name\":\"tester\",\"count\":2,\"items\":[\"a\",\"b\"]}";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    cJSON *extra_arr = cJSON_CreateArray();
    cJSON *one = cJSON_CreateString("one");
    cJSON *two = cJSON_CreateString("two");
    cJSON_AddItemToArray(extra_arr, one);
    cJSON_AddItemToArray(extra_arr, two);
    cJSON_AddItemToObject(root, "extra", extra_arr);
    cJSON *shared = cJSON_CreateString("shared_value");
    cJSON *shared_dup = cJSON_Duplicate(shared, 1);
    cJSON_AddItemToObject(root, "shared", shared_dup);
    cJSON *ref_arr = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(ref_arr, shared);
    cJSON_AddItemToObject(root, "references", ref_arr);

    // step 2: Configure
    char *updated = cJSON_SetValuestring(shared, "modified_shared");
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON *has_item_num = cJSON_CreateNumber((double)has_items);
    cJSON_AddItemToObject(root, "has_items", has_item_num);

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    size_t printed_len = printed ? strlen(printed) : 0;
    void *scratch = cJSON_malloc(printed_len + 32);
    memset(scratch, 0, printed_len + 32);
    memcpy(scratch, printed ? printed : "", printed_len);

    // step 4: Validate & Cleanup
    double extracted_has = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "has_items"));
    (void)extracted_has;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(shared);
    // API sequence test completed successfully
    return 66;
}