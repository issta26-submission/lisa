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
//<ID> 709
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    char *printed = NULL;
    size_t printed_len = 0;
    cJSON *objA = cJSON_CreateObject();
    cJSON *objB = cJSON_CreateObject();
    cJSON *numA = NULL;
    cJSON *trueA = NULL;
    cJSON *falseA = NULL;
    cJSON *numB = NULL;
    cJSON *trueB = NULL;
    cJSON *falseB = NULL;
    cJSON *got_true = NULL;
    cJSON_bool is_true = 0;
    cJSON_bool equal = 0;

    // step 2: Setup / Configure
    numA = cJSON_AddNumberToObject(objA, "value", 123.0);
    trueA = cJSON_CreateTrue();
    cJSON_AddItemToObject(objA, "flag", trueA);
    falseA = cJSON_AddFalseToObject(objA, "off");
    numB = cJSON_AddNumberToObject(objB, "value", 123.0);
    trueB = cJSON_CreateTrue();
    cJSON_AddItemToObject(objB, "flag", trueB);
    falseB = cJSON_AddFalseToObject(objB, "off");

    // step 3: Operate / Validate
    got_true = cJSON_GetObjectItem(objA, "flag");
    is_true = cJSON_IsTrue(got_true);
    equal = cJSON_Compare(objA, objB, 1);
    printed = cJSON_PrintUnformatted(objA);
    printed_len = printed ? strlen(printed) : 0;
    memcpy(scratch, printed ? printed : "", printed_len);
    scratch[printed_len] = '\0';
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(objA);
    cJSON_Delete(objB);

    // API sequence test completed successfully
    return 66;
}