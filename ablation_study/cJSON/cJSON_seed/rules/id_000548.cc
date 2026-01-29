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
//<ID> 548
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(512);
    memset(buffer, 0, 512);
    const char *json = "{\"person\":{\"name\":\"Bob\",\"age\":25},\"status\":\"active\"}";
    cJSON *root = cJSON_Parse(json);
    cJSON *pi_item = cJSON_CreateNumber(3.1415);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", pi_item);
    cJSON_AddNumberToObject(root, "score", 100);

    // step 3: Operate / Validate
    char *printed = cJSON_Print(root);
    size_t printed_len = strlen(printed);
    size_t copy_len = printed_len > 508 ? 508 : printed_len;
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *age_item = cJSON_GetObjectItem(person, "age");
    double age = cJSON_GetNumberValue(age_item);
    int age_digit = (int)age % 10;
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + age_digit);
    memcpy(buffer + 2, printed, copy_len);
    buffer[2 + copy_len] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}