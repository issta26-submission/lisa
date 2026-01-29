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
//<ID> 1010
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *list = NULL;
    cJSON *numarr = NULL;
    cJSON *raw_field = NULL;
    cJSON *flag_field = NULL;
    cJSON *to_replace = NULL;
    cJSON *replacement = NULL;
    char *printed = NULL;
    double values[3];
    cJSON_bool flag_true = 0;
    values[0] = 1.5;
    values[1] = 2.5;
    values[2] = 3.5;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    numarr = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(root, "numbers", numarr);
    flag_field = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag_field);
    raw_field = cJSON_CreateRaw("{\"rawinner\":123}");
    cJSON_AddItemToObject(root, "raw", raw_field);

    // step 3: Operate / Validate
    to_replace = cJSON_GetArrayItem(numarr, 1);
    replacement = cJSON_CreateRaw("99.9");
    cJSON_ReplaceItemViaPointer(numarr, to_replace, replacement);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}