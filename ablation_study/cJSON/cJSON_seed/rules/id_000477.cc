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
//<ID> 477
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
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *parsed = NULL;
    char *printed = NULL;
    cJSON *retr_arr = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "vals", arr);
    cJSON_AddStringToObject(root, "name", "example");

    // step 3: Operate / Validate
    printed = cJSON_PrintBuffered(root, 256, 1);
    parsed = cJSON_Parse(printed);
    retr_arr = cJSON_GetObjectItem(parsed, "vals");
    arr_size = cJSON_GetArraySize(retr_arr);
    buffer[0] = (char)('0' + (arr_size % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}