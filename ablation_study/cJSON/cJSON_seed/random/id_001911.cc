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
//<ID> 1911
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    double val1 = 0.0;
    double val2 = 0.0;
    cJSON_bool add1 = 0;
    cJSON_bool add2 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num1 = cJSON_CreateNumber(7.0);
    num2 = cJSON_CreateNumber(5.5);

    // step 3: Core operations
    add1 = cJSON_AddItemToObject(root, "first", num1);
    add2 = cJSON_AddItemToObject(root, "second", num2);
    cJSON_SetValuestring(num2, "5.5-text");
    val1 = cJSON_GetNumberValue(num1);
    val2 = cJSON_GetNumberValue(num2);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(add1 != 0);
    validation ^= (int)(add2 != 0);
    validation ^= (int)(val1 == 7.0);
    validation ^= (int)(val2 == 5.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}