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
//<ID> 2013
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *n3 = NULL;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    char *printed = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    int validation = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(3.14);
    n2 = cJSON_CreateNumber(42.0);
    n3 = cJSON_CreateNumber(-7.5);
    added1 = cJSON_AddItemToArray(arr, n1);
    added2 = cJSON_AddItemToArray(arr, n2);
    added3 = cJSON_AddItemToArray(arr, n3);

    // step 3: Core operations
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    v2 = cJSON_GetNumberValue(item2);
    printed = cJSON_PrintUnformatted(arr);

    // step 4: Operate / Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(n3 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added3 != 0);
    validation ^= (int)(item0 != NULL);
    validation ^= (int)(item1 != NULL);
    validation ^= (int)(item2 != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(v0 == 3.14);
    validation ^= (int)(v1 == 42.0);
    validation ^= (int)(v2 == -7.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(arr);

    // step 6
    // API sequence test completed successfully
    return 66;
}