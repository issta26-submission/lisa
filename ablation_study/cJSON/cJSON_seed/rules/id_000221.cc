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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *label = cJSON_CreateString("sample");
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    char *printed = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON *got_label = NULL;
    char *svalue = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "arr", array);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 1));
    got_label = cJSON_GetObjectItem(root, "label");
    svalue = cJSON_GetStringValue(got_label);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed[0];
    (void)val0;
    (void)val1;
    (void)svalue;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}