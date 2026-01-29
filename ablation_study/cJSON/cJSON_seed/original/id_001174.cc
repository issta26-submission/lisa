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
//<ID> 1174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *jsonA = "{\"num\": 42, \"text\":\"hello\"}";
    const char *jsonB = "{\"pnum\": 7} trailing";
    const char *parse_end = NULL;
    cJSON *parsedA = cJSON_ParseWithOpts(jsonA, NULL, 1);
    cJSON *parsedB = cJSON_ParseWithOpts(jsonB, &parse_end, 0);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddItemToObject(root, "parsedA", parsedA);
    cJSON_AddItemToObject(root, "parsedB", parsedB);
    cJSON *raw_item = cJSON_CreateRaw("UNPARSED_RAW");
    cJSON_AddItemToObject(root, "raw", raw_item);
    double numbers[] = {3.14, 2.718, 1.618};
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "double_array", double_array);

    // step 3: Operate
    cJSON *num_item = cJSON_GetObjectItem(parsedA, "num");
    double num_value = cJSON_GetNumberValue(num_item);
    cJSON *double_item = cJSON_CreateNumber(num_value * 2.0);
    cJSON_AddItemToObject(root, "double_of_num", double_item);
    cJSON_bool num_is_number = cJSON_IsNumber(num_item);
    cJSON_AddBoolToObject(root, "num_is_number", num_is_number);
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}