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
//<ID> 2130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *greeting = cJSON_AddStringToObject(root, "greeting", "hello_world");

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *num = cJSON_CreateNumber(100.0);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToArray(array, s1);
    cJSON_AddItemToArray(array, s2);
    cJSON_AddItemToArray(array, num);
    cJSON_AddItemToArray(array, nul);

    // step 3: Operate and Validate
    char *newstr = cJSON_SetValuestring(s2, "gamma");
    const char *fetched = cJSON_GetStringValue(s2);
    double fetched_num = cJSON_GetNumberValue(num);
    cJSON_DeleteItemFromArray(array, 2);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = fetched ? fetched[0] : '\0';
    buffer[2] = (char)('0' + ((int)fetched_num % 10));
    buffer[3] = newstr ? newstr[0] : '\0';
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}