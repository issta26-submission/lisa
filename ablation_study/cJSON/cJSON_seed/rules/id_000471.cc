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
//<ID> 471
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
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *meta = cJSON_CreateObject();
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retr_arr = NULL;
    cJSON *retr_meta = NULL;
    cJSON *retr_greet = NULL;
    const char *greet_str = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddStringToObject(meta, "greet", "hello");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 1024, 0);
    parsed = cJSON_Parse(printed);
    retr_arr = cJSON_GetObjectItem(parsed, "items");
    arr_size = cJSON_GetArraySize(retr_arr);
    retr_meta = cJSON_GetObjectItem(parsed, "meta");
    retr_greet = cJSON_GetObjectItem(retr_meta, "greet");
    greet_str = cJSON_GetStringValue(retr_greet);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = ':';
    buffer[2] = (greet_str && greet_str[0]) ? greet_str[0] : 'x';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}