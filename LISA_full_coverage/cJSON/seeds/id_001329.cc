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
//<ID> 1329
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"nums\":[1,2,3],\"meta\":{\"name\":\"sample\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *nums = cJSON_GetObjectItemCaseSensitive(parsed, "nums");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *extras = cJSON_AddArrayToObject(root, "extras");
    cJSON *newnum = cJSON_CreateNumber(42);
    cJSON_InsertItemInArray(nums, 1, newnum);

    // step 3: Operate
    cJSON *first = cJSON_GetArrayItem(nums, 0);
    cJSON_SetNumberHelper(first, 1.5);
    cJSON *ref_first = cJSON_CreateObjectReference(first);
    cJSON_AddItemReferenceToArray(extras, ref_first);
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buf);
    char *copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, buf, printed_len + 1);
    cJSON_Minify(copy);
    cJSON_free(copy);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}