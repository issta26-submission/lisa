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
//<ID> 1327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"numbers\":[1,2,3],\"info\":{\"val\":10}}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *numbers = cJSON_GetObjectItemCaseSensitive(parsed, "numbers");
    cJSON *new_num = cJSON_CreateNumber(99.0);
    cJSON_InsertItemInArray(numbers, 1, new_num);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON_AddItemReferenceToArray(refs, new_num);
    cJSON *info = cJSON_GetObjectItemCaseSensitive(parsed, "info");
    cJSON *val = cJSON_GetObjectItemCaseSensitive(info, "val");
    double updated_value = cJSON_SetNumberHelper(val, 42.5);

    // step 3: Operate
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    cJSON_bool fmt = 1;
    cJSON_PrintPreallocated(root, buffer, buf_len, fmt);
    size_t printed_len = strlen(buffer);
    char *copy_buf = (char *)cJSON_malloc(printed_len + 1);
    memset(copy_buf, 0, printed_len + 1);
    memcpy(copy_buf, buffer, printed_len + 1);
    cJSON_Minify(copy_buf);

    // step 4: Validate & Cleanup
    cJSON_free(copy_buf);
    cJSON_free(buffer);
    (void)updated_value;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}