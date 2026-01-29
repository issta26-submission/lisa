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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *objA = cJSON_CreateObject();
    cJSON *objB = cJSON_CreateObject();
    cJSON *numA = cJSON_CreateNumber(3.1415);
    cJSON *rawA = cJSON_CreateRaw("{\"embedded\":true}");
    char *printed = NULL;
    cJSON_bool equalAB = 0;
    cJSON_bool equalRootA = 0;
    double pi_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(objA, "pi", numA);
    cJSON_AddItemToObject(objA, "meta", rawA);
    cJSON_AddItemToObject(objB, "pi", cJSON_CreateNumber(3.1415));
    cJSON_AddItemToObject(objB, "meta", cJSON_CreateRaw("{\"embedded\":true}"));
    cJSON_AddItemToObject(root, "A", objA);
    cJSON_AddItemToObject(root, "B", objB);

    // step 3: Operate / Validate
    equalAB = cJSON_Compare(objA, objB, 1);
    equalRootA = cJSON_Compare(root, objA, 0);
    pi_value = cJSON_GetNumberValue(cJSON_GetObjectItem(objA, "pi"));
    cJSON_AddNumberToObject(root, "pi_value", pi_value);
    cJSON_AddNumberToObject(root, "equalAB", (double)equalAB);
    cJSON_AddNumberToObject(root, "equalRootA", (double)equalRootA);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}