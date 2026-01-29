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
//<ID> 1328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"values\":[1,2,3],\"name\":\"example\"}";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON *values = cJSON_GetObjectItem(parsed, "values");
    cJSON *new_num = cJSON_CreateNumber(99.0);
    double set_val = cJSON_SetNumberHelper(new_num, 100.5);
    cJSON_AddNumberToObject(root, "set_val", set_val);
    cJSON *inserted = cJSON_CreateString("inserted");
    cJSON_AddItemReferenceToArray(values, new_num);
    cJSON_InsertItemInArray(values, 1, inserted);

    // step 3: Operate
    int buf_len = 512;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    cJSON_bool fmt = 1;
    cJSON_PrintPreallocated(root, buf, buf_len, fmt);

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