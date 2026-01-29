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
//<ID> 979
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
    cJSON *dup = NULL;
    cJSON *values_arr = NULL;
    cJSON *readings = NULL;
    cJSON *first_item = NULL;
    char *unformatted = NULL;
    char *preallocated_buf = NULL;
    const char *version = NULL;
    const char json_text[] = "{\"device\":\"therm\",\"values\":[21.5,22.0],\"active\":true}";
    const char *parse_end = NULL;
    size_t json_len = (size_t)(sizeof(json_text) - 1);
    int pre_len = 256;
    double first_value = 0.0;
    cJSON_bool pre_ok = 0;

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "source", "unitA");
    readings = cJSON_AddArrayToObject(root, "readings");
    cJSON_AddItemToArray(readings, cJSON_CreateNumber(21.5));
    cJSON_AddItemToArray(readings, cJSON_CreateNumber(22.0));
    dup = cJSON_Duplicate(parsed, 1);
    cJSON_AddItemToObject(root, "sensor_snapshot", dup);

    // step 3: Operate / Validate
    version = cJSON_Version();
    unformatted = cJSON_PrintUnformatted(root);
    preallocated_buf = (char *)cJSON_malloc((size_t)pre_len);
    memset(preallocated_buf, 0, (size_t)pre_len);
    pre_ok = cJSON_PrintPreallocated(root, preallocated_buf, pre_len, 0);
    values_arr = cJSON_GetObjectItem(parsed, "values");
    first_item = cJSON_GetArrayItem(values_arr, 0);
    first_value = cJSON_GetNumberValue(first_item);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(preallocated_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}