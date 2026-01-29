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
//<ID> 2012
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got_item = NULL;
    char *printed = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    double extracted = 0.0;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.1415);
    num2 = cJSON_CreateNumber(2.7180);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got_item);
    printed = cJSON_PrintUnformatted(arr);

    // step 5: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(extracted > 0.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}