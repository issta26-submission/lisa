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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    cJSON *values_parsed = NULL;
    cJSON *item0 = NULL;
    char *printed = NULL;
    double val0 = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemReferenceToArray(arr, num);
    cJSON_AddItemToObject(meta, "values", arr);
    cJSON_AddStringToObject(root, "title", "example");

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_ParseWithLengthOpts(printed, (size_t)strlen(printed), &parse_end, 1);
    values_parsed = cJSON_GetObjectItem(parsed, "meta");
    values_parsed = cJSON_GetObjectItem(values_parsed, "values");
    item0 = cJSON_GetArrayItem(values_parsed, 0);
    val0 = cJSON_GetNumberValue(item0);
    buffer[0] = (char)('0' + ((int)val0 % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}