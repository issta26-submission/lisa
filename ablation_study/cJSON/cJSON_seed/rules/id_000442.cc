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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(123.75);
    cJSON *parsed = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_arr = NULL;
    cJSON *item0 = NULL;
    char *printed = NULL;
    const char *parse_end = NULL;
    int arr_size = 0;
    double first_val = 0.0;
    const char *title_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddStringToObject(root, "title", "cJSON Demo");
    cJSON_AddStringToObject(meta, "note", "reference test");
    cJSON_AddItemToArray(arr, cJSON_Duplicate(num, 1));
    cJSON_AddItemToObject(meta, "values", arr);
    cJSON_Delete(num);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_arr = cJSON_GetObjectItem(retr_meta, "values");
    arr_size = cJSON_GetArraySize(retr_arr);
    item0 = cJSON_GetArrayItem(retr_arr, 0);
    first_val = cJSON_GetNumberValue(item0);
    title_str = cJSON_GetStringValue(cJSON_GetObjectItem(parsed, "title"));
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = (char)('A' + ((int)first_val % 26));
    buffer[2] = title_str[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}