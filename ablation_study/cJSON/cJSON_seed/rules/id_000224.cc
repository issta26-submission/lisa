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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *array = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.5);
    cJSON *n2 = cJSON_CreateNumber(2.5);
    cJSON *s = cJSON_CreateString("greet");
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    char *printed = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    char *sval = NULL;
    cJSON_bool printed_ok = 0;
    cJSON *sum_node = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToArray(array, s);

    // step 3: Operate / Validate
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 0));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(array, 1));
    sum = v1 + v2;
    sum_node = cJSON_CreateNumber(sum);
    cJSON_AddItemToArray(array, sum_node);
    sval = cJSON_GetStringValue(cJSON_GetArrayItem(array, 2));
    printed = cJSON_PrintUnformatted(array);
    printed_ok = cJSON_PrintPreallocated(array, buffer, 256, 1);
    buffer[0] = printed[0];
    buffer[1] = sval[0];
    (void)printed_ok;
    (void)v1;
    (void)v2;
    (void)sum;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}