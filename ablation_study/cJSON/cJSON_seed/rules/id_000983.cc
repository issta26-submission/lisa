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
//<ID> 983
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *arr = NULL;
    cJSON *first_item = NULL;
    char *json_buf = NULL;
    char *printed = NULL;
    const char raw_json[] = "{\"device\":\"sensor\",\"ok\":false,\"values\":[10,20,30]}";
    size_t buf_len = (size_t)(sizeof(raw_json));
    double first_value = 0.0;
    int first_int = 0;

    // step 2: Setup / Configure
    json_buf = (char *)cJSON_malloc(buf_len);
    memset(json_buf, 0, buf_len);
    memcpy(json_buf, raw_json, buf_len);
    cJSON_Minify(json_buf);
    parsed = cJSON_Parse(json_buf);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "snapshot", parsed);
    cJSON_AddItemToObject(root, "alive", cJSON_CreateFalse());

    // step 3: Operate / Validate
    arr = cJSON_GetObjectItem(parsed, "values");
    first_item = cJSON_GetArrayItem(arr, 0);
    first_value = cJSON_GetNumberValue(first_item);
    first_int = (int)first_value;
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(json_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}