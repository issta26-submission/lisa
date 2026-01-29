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
//<ID> 1943
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
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_lucky = NULL;
    cJSON *got_arr0 = NULL;
    char *out = NULL;
    double pi_val = 0.0;
    double lucky_val = 0.0;
    double arr0_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num1 = cJSON_CreateNumber(7.0);
    num2 = cJSON_CreateNumber(13.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "lucky", cJSON_CreateNumber(42.0));
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    got_lucky = cJSON_GetObjectItem(root, "lucky");
    lucky_val = cJSON_GetNumberValue(got_lucky);
    got_arr0 = cJSON_GetArrayItem(arr, 0);
    arr0_val = cJSON_GetNumberValue(got_arr0);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_lucky != NULL);
    validation ^= (int)(got_arr0 != NULL);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(lucky_val == 42.0);
    validation ^= (int)(arr0_val == 7.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}