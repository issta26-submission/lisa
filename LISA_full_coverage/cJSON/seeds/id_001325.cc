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
//<ID> 1325
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3],\"meta\":{\"tag\":\"test\"}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(parsed, "numbers");
    cJSON *first_number = cJSON_GetArrayItem(numbers, 0);
    cJSON *insert_num = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(numbers, 1, insert_num);
    cJSON *ref_source = cJSON_CreateNumber(777.0);
    cJSON_AddItemToObject(parsed, "ref_source", ref_source);
    cJSON_AddItemReferenceToArray(numbers, ref_source);
    cJSON *helper_num = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(helper_num, 3.1415);
    cJSON_AddItemToArray(numbers, helper_num);

    // step 3: Operate
    int buf_len = 1024;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(parsed, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(buf);
    char *buf_copy = (char *)cJSON_malloc(printed_len + 1);
    memset(buf_copy, 0, printed_len + 1);
    memcpy(buf_copy, buf, printed_len + 1);
    cJSON_Minify(buf_copy);
    cJSON_free(buf);
    cJSON_free(buf_copy);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}