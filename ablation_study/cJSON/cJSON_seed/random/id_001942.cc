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
//<ID> 1942
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
    cJSON *num_pi = NULL;
    cJSON *num_lucky = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_lucky = NULL;
    char *out = NULL;
    double pi_val = 0.0;
    double lucky_val = 0.0;
    cJSON_bool added_values = 0;
    cJSON_bool added_lucky = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_values = cJSON_AddItemToObject(root, "values", arr);
    num_pi = cJSON_AddNumberToObject(root, "pi", 3.1415);
    num_lucky = cJSON_CreateNumber(7.0);
    added_lucky = cJSON_AddItemToObject(root, "lucky", num_lucky);
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    got_lucky = cJSON_GetObjectItem(root, "lucky");
    lucky_val = cJSON_GetNumberValue(got_lucky);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_pi != NULL);
    validation ^= (int)(num_lucky != NULL);
    validation ^= (int)(added_values != 0);
    validation ^= (int)(added_lucky != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(lucky_val == 7.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}